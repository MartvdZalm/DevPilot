#include "ModuleListItem.h"
#include "../styles/ButtonStyle.h"
#include "../styles/GroupBoxStyle.h"
#include "../styles/InputStyle.h"
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>

ModuleListItem::ModuleListItem(const Module& module, QWidget* parent)
    : QGroupBox(parent), module(module)
{
    setStyleSheet(GroupBoxStyle::primary());

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 5, 15, 15);

    QHBoxLayout* infoLayout = new QHBoxLayout();

    QVBoxLayout* infoTextLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel(module.getName());
    nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #ffffff;");
    infoTextLayout->addWidget(nameLabel);

    portLabel = new QLabel(QString("Port: %1").arg(module.getPort()));
    portLabel->setStyleSheet("font-size: 12px; color: #a0a0a0;");

    statusLabel = new QLabel("Stopped");
    statusLabel->setStyleSheet("font-size: 12px; color: #ff6b6b; font-weight: bold;");

    startButton = new QPushButton(QIcon(":/Images/Play"), "");
    startButton->setStyleSheet(ButtonStyle::primary());
    startButton->setMaximumWidth(60);
   
    stopButton = new QPushButton(QIcon(":/Images/Stop"), "");
    stopButton->setStyleSheet(ButtonStyle::primary());
    stopButton->setMaximumWidth(60);
    
    editButton = new QPushButton(QIcon(":/Images/Edit"), "");
    editButton->setStyleSheet(ButtonStyle::primary());
    editButton->setMaximumWidth(50);
    
    deleteButton = new QPushButton(QIcon(":/Images/Delete"), "");
    deleteButton->setStyleSheet(ButtonStyle::danger());
    deleteButton->setMaximumWidth(60);

    infoLayout->addLayout(infoTextLayout);
    infoLayout->addStretch();
    infoLayout->addWidget(portLabel);
    infoLayout->addWidget(statusLabel);
    infoLayout->addWidget(startButton);
    infoLayout->addWidget(stopButton);
    infoLayout->addWidget(editButton);
    infoLayout->addWidget(deleteButton);

    mainLayout->addLayout(infoLayout);

    logs = new QTextEdit();
    logs->setMaximumHeight(100);
    logs->setStyleSheet(InputStyle::commandLine());
    logs->setReadOnly(true);

    mainLayout->addWidget(logs);
    
    connect(startButton, &QPushButton::clicked, this, &ModuleListItem::startCommand);
    connect(stopButton, &QPushButton::clicked, this, &ModuleListItem::stopCommand);
    connect(editButton, &QPushButton::clicked, this, [this]() { emit editRequested(this->module); });
    connect(deleteButton, &QPushButton::clicked, this, [this]() { emit deleteRequested(this->module); });
    
    updateStatus();
}

QString ModuleListItem::cleanAnsi(const QString& text)
{
    QRegularExpression ansiEscape("\\x1B\\[[0-9;]*[A-Za-z]");
    QString result = text;
    result.replace(ansiEscape, "");
    return result;
}

void ModuleListItem::setLogs(const QString& text)
{
    logs->setPlainText(text);
}

void ModuleListItem::appendLog(const QString& line)
{
    logs->append(cleanAnsi(line));
}

void ModuleListItem::setModule(const Module& module)
{
    this->module = module;
    updateStatus();
}

void ModuleListItem::updateStatus()
{
    if (!statusLabel) return;
    
    QString statusText;
    QString statusColor;
    
    switch (module.getStatus())
    {
        case Module::Status::Stopped:
            statusText = "Stopped";
            statusColor = "#ff6b6b";
            startButton->setEnabled(true);
            stopButton->setEnabled(false);
            break;
        case Module::Status::Starting:
            statusText = "Starting...";
            statusColor = "#ffa726";
            startButton->setEnabled(false);
            stopButton->setEnabled(true);
            break;
        case Module::Status::Running:
            statusText = "Running";
            statusColor = "#66bb6a";
            startButton->setEnabled(false);
            stopButton->setEnabled(true);
            break;
        case Module::Status::Stopping:
            statusText = "Stopping...";
            statusColor = "#ffa726";
            startButton->setEnabled(false);
            stopButton->setEnabled(false);
            break;
        case Module::Status::Error:
            statusText = "Error";
            statusColor = "#f44336";
            startButton->setEnabled(true);
            stopButton->setEnabled(false);
            break;
        default:
            statusText = "Unknown";
            statusColor = "#9e9e9e";
            startButton->setEnabled(true);
            stopButton->setEnabled(false);
            break;
    }
    
    statusLabel->setText(statusText);
    statusLabel->setStyleSheet(QString("font-size: 12px; color: %1; font-weight: bold;").arg(statusColor));
}

Module ModuleListItem::getModule() const
{
    return module;
}

void ModuleListItem::startCommand()
{
    if (process)
    {
        appendLog("⚠ Process already running.");
        return;
    }

    process = new QProcess(this);
    module.setStatus(Module::Status::Starting);
    updateStatus();

    if (!module.getWorkingDirectory().isEmpty())
        process->setWorkingDirectory(module.getWorkingDirectory());

    connect(process, &QProcess::readyReadStandardOutput, this,
            [this]() { appendLog(QString::fromUtf8(process->readAllStandardOutput())); });
    connect(process, &QProcess::readyReadStandardError, this,
            [this]() { appendLog(QString::fromUtf8(process->readAllStandardError())); });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this](int exitCode, QProcess::ExitStatus)
            {
                if (exitCode == 0)
                {
                    appendLog(QString("✅ Process finished successfully"));
                    module.setStatus(Module::Status::Stopped);
                }
                else
                {
                    appendLog(QString("❌ Process finished with error code %1").arg(exitCode));
                    module.setStatus(Module::Status::Error);
                }
                process = nullptr;
                updateStatus();
            });
    connect(process, &QProcess::started, this,
            [this]()
            {
                module.setStatus(Module::Status::Running);
                updateStatus();
            });

    appendLog(QString("▶ Running: %1").arg(module.getCommand()));

#ifdef _WIN32
    process->start("cmd.exe", {"/C", module.getCommand()});
#else
    process->start("bash", {"-c", module.getCommand()});
#endif
}

void ModuleListItem::stopCommand()
{
    if (!process)
        return;

    module.setStatus(Module::Status::Stopping);
    updateStatus();
    appendLog("⏹ Stopping process...");

#ifdef _WIN32
    QProcess::execute("taskkill", {"/PID", QString::number(process->processId()), "/T", "/F"});
#else
    ::kill(-process->processId(), SIGINT);
#endif

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this](int exitCode, QProcess::ExitStatus)
            {
                appendLog(QString("✅ Process stopped"));
                module.setStatus(Module::Status::Stopped);
                process = nullptr;
                updateStatus();
            });
}

QPushButton* ModuleListItem::getStartButton()
{
    return startButton;
}

QPushButton* ModuleListItem::getStopButton()
{
    return stopButton;
}

QTextEdit* ModuleListItem::getLogs()
{
    return logs;
}
