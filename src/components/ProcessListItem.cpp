#include "ProcessListItem.h"
#include "../styles/ButtonStyle.h"
#include "../styles/GroupBoxStyle.h"
#include "../windows/ProcessWindow.h"
#include "../core/Logger.h"
#include <QDir>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>

ProcessListItem::ProcessListItem(Process& process, IProcessRepository& processRepository, QWidget* parent)
    : QGroupBox(parent), process(process), processRepository(processRepository)
{
    setStyleSheet(GroupBoxStyle::primary());
    setupUI();
    setupConnections();

    // Check if process is actually running when the list item is created
    if (process.getStatus() == Process::Status::Running && process.getPID() > 0)
    {
        if (!isProcessRunning(process.getPID()))
        {
            // PID is no longer valid, check by port
            int pidFromPort = findPidByPort(process.getPort());
            if (pidFromPort > 0)
            {
                // Update with the correct PID
                process.setPID(pidFromPort);
                processRepository.save(process);
            }
            else
            {
                // Process is not actually running
                process.setStatus(Process::Status::Stopped);
                process.setPID(0);
                processRepository.save(process);
            }
        }
    }

    updateStatus();

    // Start background monitoring for running processes
    if (process.getStatus() == Process::Status::Running)
    {
        startBackgroundMonitoring();
    }
}

ProcessListItem::~ProcessListItem()
{
    prepareForShutdown();

    // Clean up QProcess without waiting
    if (qProcess)
    {
        qProcess->deleteLater();
    }
}

void ProcessListItem::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 5, 15, 15);

    QHBoxLayout* infoLayout = new QHBoxLayout();

    QVBoxLayout* infoTextLayout = new QVBoxLayout();
    QLabel* nameLabel = new QLabel(process.getName());
    nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #ffffff;");
    infoTextLayout->addWidget(nameLabel);

    portLabel = new QLabel(QString("Port: %1").arg(process.getPort()));
    portLabel->setStyleSheet("font-size: 12px; color: #a0a0a0;");

    statusLabel = new QLabel();
    statusLabel->setStyleSheet("font-size: 12px; color: #ff6b6b; font-weight: bold;");

    startButton = new QPushButton(QIcon(":/Images/Play"), "");
    startButton->setStyleSheet(ButtonStyle::primary());
    startButton->setMaximumWidth(60);

    stopButton = new QPushButton(QIcon(":/Images/Stop"), "");
    stopButton->setStyleSheet(ButtonStyle::primary());
    stopButton->setMaximumWidth(60);

    terminalButton = new QPushButton(QIcon(":/Images/Terminal"), "");
    terminalButton->setStyleSheet(ButtonStyle::primary());
    terminalButton->setMaximumWidth(60);
    terminalButton->setToolTip("Open in Terminal Window");

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
    infoLayout->addWidget(terminalButton);
    infoLayout->addWidget(editButton);
    infoLayout->addWidget(deleteButton);

    mainLayout->addLayout(infoLayout);
}

void ProcessListItem::setupConnections()
{
    connect(startButton, &QPushButton::clicked, this, &ProcessListItem::startCommand);
    connect(stopButton, &QPushButton::clicked, this, &ProcessListItem::stopCommand);
    connect(terminalButton, &QPushButton::clicked, this, &ProcessListItem::openTerminalWindow);
    connect(editButton, &QPushButton::clicked, this, [this]() { emit editRequested(this->process); });
    connect(deleteButton, &QPushButton::clicked, this, [this]() { emit deleteRequested(this->process); });
}

void ProcessListItem::setProcess(Process& process)
{
    this->process = process;
    updateStatus();
}

void ProcessListItem::updateStatus()
{
    if (!statusLabel)
        return;

    QString statusText;
    QString statusColor;

    switch (process.getStatus())
    {
    case Process::Status::Stopped:
        statusText = "Stopped";
        statusColor = "#ff6b6b";
        break;
    case Process::Status::Starting:
        statusText = "Starting...";
        statusColor = "#ffa726";
        break;
    case Process::Status::Running:
        statusText = QString("Running (PID: %1)").arg(process.getPID());
        statusColor = "#66bb6a";
        break;
    case Process::Status::Error:
        statusText = "Error";
        statusColor = "#f44336";
        break;
    default:
        statusText = "Unknown";
        statusColor = "#9e9e9e";
        break;
    }

    statusLabel->setText(statusText);
    statusLabel->setStyleSheet(QString("font-size: 12px; color: %1; font-weight: bold;").arg(statusColor));

    startButton->setEnabled(process.getStatus() == Process::Status::Stopped ||
                            process.getStatus() == Process::Status::Error);
    stopButton->setEnabled(process.getStatus() == Process::Status::Running ||
                           process.getStatus() == Process::Status::Starting);
}

Process ProcessListItem::getProcess()
{
    return process;
}

void ProcessListItem::startCommand()
{
    if (qProcess)
    {
        return;
    }

    qProcess = new QProcess(this);
    process.setStatus(Process::Status::Starting);
    updateStatus();

    if (!process.getWorkingDirectory().isEmpty())
    {
        qProcess->setWorkingDirectory(process.getWorkingDirectory());
    }

    QString logsDirPath = QDir::current().absoluteFilePath("logs");
    QDir logsDir(logsDirPath);
    if (!logsDir.exists())
    {
        logsDir.mkpath(".");
    }

    QString logFilePath = logsDir.filePath(process.getName() + ".log");
    qProcess->setProcessChannelMode(QProcess::MergedChannels);

    QFile logHeader(logFilePath);
    if (logHeader.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&logHeader);
        out << "\n\n===== Starting process: " << process.getName() << " ("
            << QDateTime::currentDateTime().toString(Qt::ISODate) << ") =====\n";
    }

    connect(qProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this](int exitCode, QProcess::ExitStatus exitStatus) { handleProcessFinished(exitCode, exitStatus); });

    connect(qProcess, &QProcess::started, this, [this]() { handleProcessStarted(); });

    connect(qProcess, &QProcess::errorOccurred, this,
            [this](QProcess::ProcessError error)
            {
                // Don't mark as error if we're shutting down
                if (!isShuttingDown)
                {
                    process.setStatus(Process::Status::Error);
                    processRepository.save(process);
                    updateStatus();
                }
            });

    connect(qProcess, &QProcess::readyReadStandardOutput, this,
            [this, logFilePath]()
            {
                QByteArray rawData = qProcess->readAllStandardOutput();

                QFile logFile(logFilePath);
                if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
                {
                    QTextStream out(&logFile);
                    out << QString::fromUtf8(rawData);
                }
            });

    qProcess->start("cmd.exe", {"/C", process.getCommand()});
}

void ProcessListItem::handleProcessStarted()
{
    qint64 qProcessPid = qProcess->processId();

    if (qProcessPid > 0)
    {
        // Store the QProcess PID initially
        process.setPID(qProcessPid);
        startPortPolling(); // Still use port polling to find the real application PID
    }
    else
    {
        // Fallback to port detection only
        startPortPolling();
    }
}

void ProcessListItem::startPortPolling()
{
    if (pollTimer)
    {
        pollTimer->stop();
        pollTimer->deleteLater();
    }

    pollTimer = new QTimer(this);
    int attempts = 0;
    const int maxAttempts = 30;
    const int intervalMs = 500;

    connect(pollTimer, &QTimer::timeout, this,
            [this, attempts, maxAttempts, intervalMs]() mutable
            {
                attempts++;
                int realPid = findPidByPort(process.getPort());

                if (realPid > 0)
                {
                    process.setStatus(Process::Status::Running);
                    process.setLastStartedAt(QDateTime::currentDateTime());
                    process.setPID(realPid);
                    processRepository.save(process);
                    updateStatus();

                    // Start background monitoring since we now have the real PID
                    startBackgroundMonitoring();

                    pollTimer->stop();
                    pollTimer->deleteLater();
                    pollTimer = nullptr;
                }
                else if (attempts >= maxAttempts)
                {
                    LOG_INFO("Timeout: Could not find PID for port " + QString::number(process.getPort()));

                    process.setStatus(Process::Status::Error);
                    processRepository.save(process);
                    updateStatus();
                    pollTimer->stop();
                    pollTimer->deleteLater();
                    pollTimer = nullptr;
                }
            });

    pollTimer->start(intervalMs);
}

void ProcessListItem::startBackgroundMonitoring()
{
    if (monitorTimer)
    {
        monitorTimer->stop();
        monitorTimer->deleteLater();
    }

    // Only monitor if process is running
    if (process.getStatus() != Process::Status::Running)
    {
        return;
    }

    monitorTimer = new QTimer(this);
    connect(monitorTimer, &QTimer::timeout, this,
            [this]()
            {
                // Check if the process is still running using multiple methods
                bool isRunning = false;

                // Method 1: Check by stored PID
                if (process.getPID() > 0)
                {
                    isRunning = isProcessRunning(process.getPID());
                }

                // Method 2: If PID check fails, check by port
                if (!isRunning && process.getPort() > 0)
                {
                    int pidFromPort = findPidByPort(process.getPort());
                    if (pidFromPort > 0)
                    {
                        // Update PID if it changed
                        if (pidFromPort != process.getPID())
                        {
                            process.setPID(pidFromPort);
                            processRepository.save(process);
                        }
                        isRunning = true;
                    }
                }

                // Update status if process is no longer running
                if (!isRunning)
                {
                    process.setStatus(Process::Status::Stopped);
                    process.setPID(0);
                    processRepository.save(process);
                    updateStatus();

                    monitorTimer->stop();
                    monitorTimer->deleteLater();
                    monitorTimer = nullptr;
                }
            });

    monitorTimer->start(5000);
}

void ProcessListItem::stopCommand()
{
    // Stop monitoring first
    if (monitorTimer)
    {
        monitorTimer->stop();
        monitorTimer->deleteLater();
        monitorTimer = nullptr;
    }

    // Stop port polling
    if (pollTimer)
    {
        pollTimer->stop();
        pollTimer->deleteLater();
        pollTimer = nullptr;
    }

    // Method 1: Stop by stored PID
    if (process.getPID() > 0)
    {
        QProcess::execute("taskkill", {"/PID", QString::number(process.getPID()), "/T", "/F"});
    }

    // Method 2: Stop QProcess if it exists
    if (qProcess && qProcess->state() == QProcess::Running)
    {
        qProcess->terminate();
        if (!qProcess->waitForFinished(3000))
        {
            qProcess->kill();
            qProcess->waitForFinished(1000);
        }
    }

    // Method 3: Fallback - kill by port
    int pidFromPort = findPidByPort(process.getPort());
    if (pidFromPort > 0 && pidFromPort != process.getPID())
    {
        QProcess::execute("taskkill", {"/PID", QString::number(pidFromPort), "/T", "/F"});
    }

    process.setPID(0);
    process.setStatus(Process::Status::Stopped);
    processRepository.save(process);
    updateStatus();

    // Clean up QProcess
    if (qProcess)
    {
        qProcess->deleteLater();
        qProcess = nullptr;
    }
}

void ProcessListItem::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    Q_UNUSED(exitStatus)

    // Don't immediately mark as stopped check if the actual application is still running
    if (process.getStatus() == Process::Status::Running)
    {
        // Check if the real process (by port) is still running
        int pidFromPort = findPidByPort(process.getPort());
        if (pidFromPort > 0)
        {
            // Application is still running, just update the PID
            process.setPID(pidFromPort);
            processRepository.save(process);
            updateStatus();

            // Ensure monitoring continues
            startBackgroundMonitoring();
        }
        else
        {
            // Application is actually stopped
            process.setPID(0);
            process.setStatus(Process::Status::Stopped);
            processRepository.save(process);
            updateStatus();
        }
    }

    if (qProcess)
    {
        qProcess->deleteLater();
        qProcess = nullptr;
    }
}

void ProcessListItem::prepareForShutdown()
{
    isShuttingDown = true;

    // Stop all timers
    if (pollTimer)
    {
        pollTimer->stop();
        pollTimer->deleteLater();
        pollTimer = nullptr;
    }

    if (monitorTimer)
    {
        monitorTimer->stop();
        monitorTimer->deleteLater();
        monitorTimer = nullptr;
    }

    // Don't terminate QProcess during shutdown, let the OS handle it
    // This prevents the error signals from being emitted
    if (qProcess)
    {
        // Disconnect all signals to prevent error handling during shutdown
        qProcess->disconnect();

        if (process.getStatus() == Process::Status::Starting)
        {
            process.setStatus(Process::Status::Stopped);
            processRepository.save(process);
        }
    }
}

void ProcessListItem::openTerminalWindow()
{
    ProcessWindow* window = new ProcessWindow(process);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    window->raise();
    window->activateWindow();
}

QPushButton* ProcessListItem::getStartButton()
{
    return startButton;
}

QPushButton* ProcessListItem::getStopButton()
{
    return stopButton;
}

bool ProcessListItem::isProcessRunning(int pid)
{
    QProcess process;
    process.start("tasklist", {"/FI", QString("PID eq %1").arg(pid)});
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    return output.contains(QString::number(pid));
}

int ProcessListItem::findPidByPort(int port)
{
    QString command = QString("netstat -ano | findstr :%1").arg(port);
    QProcess getPid;
    getPid.start("cmd.exe", QStringList() << "/C" << command);
    getPid.waitForFinished();

    QString output = getPid.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    QRegularExpression re("\\s+(\\d+)\\s*$");

    for (const QString& line : lines)
    {
        QString trimmed = line.trimmed();

        if (trimmed.contains("TIME_WAIT") || trimmed.contains("CLOSE_WAIT"))
        {
            continue;
        }

        QRegularExpressionMatch match = re.match(trimmed);
        if (match.hasMatch())
        {
            int pid = match.captured(1).toInt();
            if (pid != 0)
            {
                LOG_INFO("Found PID: " + QString::number(pid));
                return pid;
            }
        }
    }

    return 0;
}
