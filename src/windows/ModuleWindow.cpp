#include "ModuleWindow.h"
#include "../styles/ButtonStyle.h"
#include "../styles/InputStyle.h"
#include "../styles/ModuleStyle.h"
#include <QGroupBox>
#include <QScrollBar>
#include <QSplitter>
#include <QTabWidget>
#include <QDesktopServices>
#include <QTimer>
#include <QDir>

ModuleWindow::ModuleWindow(QWidget* parent, const Module& module)
    : BaseWindow(parent), module(module), process(new QProcess(this))
{
    setupUI();
    setupConnections();

    if (module.getStatus() == Module::Status::Running)
    {
        startProcess();
    }
}

void ModuleWindow::setupUI()
{
    setWindowTitle(module.getName() + " - Module");
    setMinimumSize(900, 650);
    resize(1100, 750);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(12);

    mainLayout->addWidget(createHeader());
    mainLayout->addLayout(createControls());
    mainLayout->addWidget(createTabWidget(), 1);
    mainLayout->addWidget(createInputSection());

    terminalOutput->append(QString("Module: %1").arg(module.getName()));
    terminalOutput->append("Ready to start. Press 'Start' to begin.\n");
}

void ModuleWindow::setupConnections()
{
    connect(startButton, &QPushButton::clicked, this, &ModuleWindow::startProcess);
    connect(stopButton, &QPushButton::clicked, this, &ModuleWindow::stopProcess);
    connect(restartButton, &QPushButton::clicked, this, &ModuleWindow::restartProcess);
    connect(clearButton, &QPushButton::clicked, this, &ModuleWindow::clearTerminal);
    connect(sendButton, &QPushButton::clicked, this, &ModuleWindow::onCommandEntered);
    connect(commandInput, &QLineEdit::returnPressed, this, &ModuleWindow::onCommandEntered);
    connect(process, &QProcess::readyRead, this, &ModuleWindow::onReadyRead);
    connect(process, &QProcess::readyReadStandardError, this, &ModuleWindow::onReadyReadError);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &ModuleWindow::onProcessFinished);
}

QWidget* ModuleWindow::createHeader()
{
    QWidget* headerWidget = new QWidget();
    headerWidget->setStyleSheet(ModuleStyle::header());
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(12, 8, 12, 8);

    QLabel* moduleName = new QLabel(module.getName());
    moduleName->setStyleSheet("color: #ffffff; font-size: 18px; font-weight: bold;");

    statusIndicator = new QLabel("Stopped");
    statusIndicator->setStyleSheet(ModuleStyle::statusIndicator(false));

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->addWidget(moduleName);
    titleLayout->addWidget(statusIndicator);
    titleLayout->setSpacing(4);

    QVBoxLayout* infoLayout = new QVBoxLayout();
    if (module.getPort() > 0)
    {
        QLabel* portLabel = new QLabel(QString("Port: %1").arg(module.getPort()));
        portLabel->setStyleSheet("color: #a0a0a0; font-size: 11px;");
        infoLayout->addWidget(portLabel);
    }

    if (!module.getWorkingDirectory().isEmpty())
    {
        QLabel* dirLabel = new QLabel(QString("Dir: %1").arg(QDir(module.getWorkingDirectory()).dirName()));
        dirLabel->setStyleSheet("color: #a0a0a0; font-size: 11px;");
        dirLabel->setToolTip(module.getWorkingDirectory());
        infoLayout->addWidget(dirLabel);
    }

    headerLayout->addLayout(titleLayout);
    headerLayout->addSpacing(20);
    headerLayout->addLayout(infoLayout);
    headerLayout->addStretch();

    return headerWidget;
}

QHBoxLayout* ModuleWindow::createControls()
{
    QHBoxLayout* controlLayout = new QHBoxLayout();

    startButton = new QPushButton("Start");
    startButton->setStyleSheet(ButtonStyle::primary());

    stopButton = new QPushButton("Stop");
    stopButton->setStyleSheet(ButtonStyle::danger());
    stopButton->setEnabled(false);

    restartButton = new QPushButton("Restart");
    restartButton->setStyleSheet(ButtonStyle::primary());
    restartButton->setEnabled(false);

    clearButton = new QPushButton("Clear");
    clearButton->setStyleSheet(ButtonStyle::primary());
    clearButton->setFixedWidth(80);

    autoScrollCheckbox = new QCheckBox("Auto-scroll");
    autoScrollCheckbox->setChecked(true);
    autoScrollCheckbox->setStyleSheet("color: #ffffff;");

    controlLayout->addWidget(startButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(restartButton);
    controlLayout->addStretch();
    controlLayout->addWidget(autoScrollCheckbox);
    controlLayout->addWidget(clearButton);

    return controlLayout;
}

QTabWidget* ModuleWindow::createTabWidget()
{
    tabWidget = new QTabWidget();
    tabWidget->setStyleSheet(ModuleStyle::tabWidget());

    QWidget* terminalTab = new QWidget();
    QVBoxLayout* terminalLayout = new QVBoxLayout(terminalTab);
    terminalLayout->setContentsMargins(0, 5, 0, 0);

    terminalOutput = new QTextEdit();
    terminalOutput->setStyleSheet(ModuleStyle::terminal());
    terminalOutput->setReadOnly(true);
    terminalLayout->addWidget(terminalOutput);
    tabWidget->addTab(terminalTab, "Terminal Output");

    QWidget* detailsTab = new QWidget();
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsTab);
    detailsLayout->setContentsMargins(15, 15, 15, 15);

    QLabel* cmdLabel = new QLabel("Command:");
    cmdLabel->setStyleSheet(ModuleStyle::sectionLabel());
    QLabel* cmdValue = new QLabel(module.getCommand());
    cmdValue->setStyleSheet(ModuleStyle::infoValue());
    cmdValue->setWordWrap(true);
    cmdValue->setTextInteractionFlags(Qt::TextSelectableByMouse);

    detailsLayout->addWidget(cmdLabel);
    detailsLayout->addWidget(cmdValue);

    if (!module.getWorkingDirectory().isEmpty())
    {
        QLabel* dirLabel = new QLabel("Working Directory:");
        dirLabel->setStyleSheet("color: white; font-weight: bold;");
        QLabel* dirValue = new QLabel(module.getWorkingDirectory());
        dirValue->setStyleSheet("color: #a0a0a0;");
        dirValue->setTextInteractionFlags(Qt::TextSelectableByMouse);

        detailsLayout->addSpacing(10);
        detailsLayout->addWidget(dirLabel);
        detailsLayout->addWidget(dirValue);
    }

    if (module.getPort() > 0)
    {
        QLabel* portLabel = new QLabel("Port:");
        portLabel->setStyleSheet("color: white; font-weight: bold;");
        QLabel* portValue = new QLabel(QString::number(module.getPort()));
        portValue->setStyleSheet("color: #a0a0a0;");

        QPushButton* openPortBtn = new QPushButton("Open in Browser");
        openPortBtn->setStyleSheet(ButtonStyle::primary());
        openPortBtn->setFixedWidth(140);
        connect(openPortBtn, &QPushButton::clicked, this,
                [this]() { QDesktopServices::openUrl(QUrl(QString("http://localhost:%1").arg(module.getPort()))); });

        QHBoxLayout* portLayout = new QHBoxLayout();
        portLayout->addWidget(portValue);
        portLayout->addWidget(openPortBtn);
        portLayout->addStretch();

        detailsLayout->addSpacing(10);
        detailsLayout->addWidget(portLabel);
        detailsLayout->addLayout(portLayout);
    }

    detailsLayout->addStretch();
    tabWidget->addTab(detailsTab, "Module Details");

    return tabWidget;
}

QWidget* ModuleWindow::createInputSection()
{
    QWidget* inputWidget = new QWidget();
    inputWidget->setStyleSheet(ModuleStyle::inputContainer());
    QHBoxLayout* inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(8, 8, 8, 8);

    QLabel* promptLabel = new QLabel("$");
    promptLabel->setStyleSheet("color: #58a6ff; font-weight: bold; font-size: 14px;");

    commandInput = new QLineEdit();
    commandInput->setStyleSheet(ModuleStyle::commandInput());
    commandInput->setPlaceholderText("Enter command...");
    commandInput->setEnabled(false);

    sendButton = new QPushButton("Send");
    sendButton->setStyleSheet(ButtonStyle::primary());
    sendButton->setFixedWidth(80);
    sendButton->setEnabled(false);

    inputLayout->addWidget(promptLabel);
    inputLayout->addWidget(commandInput);
    inputLayout->addWidget(sendButton);

    return inputWidget;
}

void ModuleWindow::startProcess()
{
    if (process->state() == QProcess::Running)
    {
        appendColoredText("Process is already running.", "#ffa657");
        return;
    }

    if (!module.getWorkingDirectory().isEmpty())
    {
        process->setWorkingDirectory(module.getWorkingDirectory());
    }

    appendColoredText("Starting process...", "#7ee787");
    appendColoredText(QString("Command: %1").arg(module.getCommand()), "#8b949e");
    terminalOutput->append("");

    process->start("cmd.exe");
    updateControlsState(true);
    updateStatusIndicator(true);
}

void ModuleWindow::stopProcess()
{
    if (process->state() != QProcess::Running)
    {
        appendColoredText("No process is running.", "#ffa657");
        return;
    }

    appendColoredText("Stopping process...", "#f85149");
#ifdef _WIN32
    process->kill();
#else
    process->terminate();
#endif
}

void ModuleWindow::restartProcess()
{
    if (process->state() == QProcess::Running)
    {
        appendColoredText("Restarting process...", "#ffa657");
        disconnect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
                   &ModuleWindow::onProcessFinished);
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
                [this](int, QProcess::ExitStatus)
                {
                    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
                            &ModuleWindow::onProcessFinished);
                    QTimer::singleShot(300, this, &ModuleWindow::startProcess);
                });
        process->terminate();
    }
    else
    {
        startProcess();
    }
}

void ModuleWindow::onReadyRead()
{
    terminalOutput->insertPlainText(QString::fromLocal8Bit(process->readAll()));
    autoScroll();
}

void ModuleWindow::onReadyReadError()
{
    appendColoredText(QString::fromLocal8Bit(process->readAllStandardError()), "#f85149");
    autoScroll();
}

void ModuleWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    terminalOutput->append("");
    if (exitCode == 0)
    {
        appendColoredText(QString("Process finished successfully (exit code: %1)").arg(exitCode), "#7ee787");
    }
    else
    {
        appendColoredText(QString("Process finished with exit code: %1").arg(exitCode), "#f85149");
    }

    terminalOutput->append("");
    updateControlsState(false);
    updateStatusIndicator(false);
    process->close();
}

void ModuleWindow::onCommandEntered()
{
    if (process->state() != QProcess::Running)
    {
        appendColoredText("No process is running. Start it first.", "#ffa657");
        return;
    }

    QString command = commandInput->text().trimmed();
    if (command.isEmpty())
    {
        return;
    }

    appendColoredText(QString("$ %1").arg(command), "#58a6ff");
    process->write(command.toLocal8Bit() + "\r\n");
    commandInput->clear();
    autoScroll();
}

void ModuleWindow::clearTerminal()
{
    terminalOutput->clear();
    terminalOutput->append(QString("Module: %1").arg(module.getName()));
    appendColoredText("Terminal cleared.\n", "#8b949e");
}

void ModuleWindow::updateControlsState(bool running)
{
    startButton->setEnabled(!running);
    stopButton->setEnabled(running);
    restartButton->setEnabled(running);
    commandInput->setEnabled(running);
    sendButton->setEnabled(running);

    if (running)
    {
        commandInput->setFocus();
    }
}

void ModuleWindow::updateStatusIndicator(bool running)
{
    statusIndicator->setText(running ? "Running" : "Stopped");
    statusIndicator->setStyleSheet(ModuleStyle::statusIndicator(running));
}

void ModuleWindow::appendColoredText(const QString& text, const QString& color)
{
    terminalOutput->append(QString("<span style='color: %1;'>%2</span>").arg(color, text));
}

void ModuleWindow::autoScroll()
{
    if (autoScrollCheckbox->isChecked())
    {
        QScrollBar* scrollBar = terminalOutput->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}