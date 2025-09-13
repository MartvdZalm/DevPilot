#include "ModuleListItem.h"
#include "../styles/ButtonStyle.h"
#include "../styles/GroupBoxStyle.h"
#include "../styles/InputStyle.h"
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>

ModuleListItem::ModuleListItem(const Module& module, const QString& iconEmoji, QWidget* parent)
    : QGroupBox(module.getName(), parent), module(module)
{
    qDebug() << module.serialize();

    setStyleSheet(GroupBoxStyle::primary());

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 20, 15, 15);

    QHBoxLayout* infoLayout = new QHBoxLayout();

    QLabel* iconLabel = new QLabel(iconEmoji);
    iconLabel->setStyleSheet("font-size: 16px;");

    QVBoxLayout* infoTextLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel(module.getName());
    nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #ffffff;");
    QLabel* logsLabel = new QLabel("Logs");
    logsLabel->setStyleSheet("font-size: 12px; color: #a0a0a0;");
    infoTextLayout->addWidget(nameLabel);
    infoTextLayout->addWidget(logsLabel);

    portLabel = new QLabel(QString("Port: %1").arg(module.getPort()));
    portLabel->setStyleSheet("font-size: 12px; color: #a0a0a0;");

    startButton = new QPushButton("Start");
    startButton->setStyleSheet(ButtonStyle::primary());
    stopButton = new QPushButton("Stop");
    stopButton->setStyleSheet(ButtonStyle::primary());

    infoLayout->addWidget(iconLabel);
    infoLayout->addLayout(infoTextLayout);
    infoLayout->addStretch();
    infoLayout->addWidget(portLabel);
    infoLayout->addWidget(startButton);
    infoLayout->addWidget(stopButton);

    mainLayout->addLayout(infoLayout);

    logs = new QTextEdit();
    logs->setMaximumHeight(100);
    logs->setStyleSheet(InputStyle::commandLine());
    logs->setReadOnly(true);

    mainLayout->addWidget(logs);
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
}

void ModuleListItem::startCommand()
{
    if (process)
    {
        appendLog("⚠ Process already running.");
        return;
    }

    process = new QProcess(this);

    if (!module.getWorkingDirectory().isEmpty())
        process->setWorkingDirectory(module.getWorkingDirectory());

    connect(process, &QProcess::readyReadStandardOutput, this,
            [this]() { appendLog(QString::fromUtf8(process->readAllStandardOutput())); });
    connect(process, &QProcess::readyReadStandardError, this,
            [this]() { appendLog(QString::fromUtf8(process->readAllStandardError())); });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this](int exitCode, QProcess::ExitStatus)
            {
                appendLog(QString("✅ Process finished with code %1").arg(exitCode));
                process = nullptr;
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

    appendLog("⏹ Stopping process...");

#ifdef _WIN32
    QProcess::execute("taskkill", {"/PID", QString::number(process->processId()), "/T", "/F"});
#else
    ::kill(-process->processId(), SIGINT);
#endif

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this](int exitCode, QProcess::ExitStatus)
            {
                appendLog(QString("✅ Process finished with code %1").arg(exitCode));
                process = nullptr;
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
