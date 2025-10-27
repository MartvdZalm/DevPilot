#include "ProcessWindow.h"
#include "../core/AnsiHtmlConverter.h"
#include "../styles/ButtonStyle.h"
#include "../styles/GroupBoxStyle.h"
#include "../styles/InputStyle.h"
#include <QApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QProcess>
#include <QScrollBar>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>

ProcessWindow::ProcessWindow(const Process& process, QWidget* parent) : BaseWindow(parent), currentProcess(process)
{
    updateTimer = new QTimer(this);
    updateTimer->setInterval(2000);

    setupUI();
    setupConnections();
    setProcess(process);

    updateTimer->start();
}

void ProcessWindow::setupUI()
{
    setWindowTitle("Process Manager - " + currentProcess.getName());
    setMinimumSize(800, 600);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QGroupBox* headerGroup = new QGroupBox("Process");
    headerGroup->setStyleSheet(GroupBoxStyle::primary());
    auto* headerLayout = new QHBoxLayout(headerGroup);

    auto* statusLayout = new QVBoxLayout();
    statusLabel = new QLabel("Status: " + currentProcess.getStatusString());
    statusLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    pidLabel = new QLabel("PID: " + QString::number(currentProcess.getPID()));
    uptimeLabel = new QLabel("Uptime: --");

    statusLayout->addWidget(statusLabel);
    statusLayout->addWidget(pidLabel);
    statusLayout->addWidget(uptimeLabel);

    headerLayout->addLayout(statusLayout);
    mainLayout->addWidget(headerGroup);

    tabWidget = new QTabWidget();
    setupLogsTab();
    setupConfigurationTab();

    tabWidget->addTab(logsTab, "Logs");
    tabWidget->addTab(configurationTab, "Configuration");

    mainLayout->addWidget(tabWidget);

    QString savedTheme = settings.value("terminal/theme", "dark").toString();

    int themeIndex = themeComboBox->findData(savedTheme);
    if (themeIndex != -1)
    {
        themeComboBox->setCurrentIndex(themeIndex);
    }

    applyTheme(savedTheme);
}

void ProcessWindow::setupLogsTab()
{
    logsTab = new QWidget();
    auto* layout = new QVBoxLayout(logsTab);

    auto* logControlsLayout = new QHBoxLayout();
    clearLogsButton = new QPushButton(QIcon(":/Images/Mop"), "");
    clearLogsButton->setStyleSheet(ButtonStyle::primary());

    themeComboBox = new QComboBox();
    themeComboBox->addItem("Dark Theme", "dark");
    themeComboBox->addItem("Light Theme", "light");
    themeComboBox->addItem("Terminal Green", "terminal");
    themeComboBox->setStyleSheet(InputStyle::primary());

    logControlsLayout->addWidget(clearLogsButton);
    logControlsLayout->addWidget(themeComboBox);
    logControlsLayout->addStretch();

    layout->addLayout(logControlsLayout);

    auto* logsLayout = new QVBoxLayout();
    logsTextEdit = new QTextBrowser();
    logsTextEdit->setReadOnly(true);
    logsTextEdit->setStyleSheet(InputStyle::primary());

    logsLayout->addWidget(logsTextEdit);
    layout->addLayout(logsLayout);

    QString logsDirPath = QDir::current().absoluteFilePath("logs");
    QDir logsDir(logsDirPath);
    if (!logsDir.exists())
    {
        logsDir.mkpath(".");
    }

    QString logFilePath = logsDir.filePath(currentProcess.getName() + ".log");
    logFile.setFileName(logFilePath);

    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        logsTextEdit->setPlainText("Failed to open log file: " + logFilePath);
    }

    lastLogPosition = logFile.pos();

    logUpdateTimer = new QTimer(this);
    logUpdateTimer->setInterval(1000);
    logUpdateTimer->start();
}

void ProcessWindow::setupConnections()
{
    connect(logUpdateTimer, &QTimer::timeout, this, &ProcessWindow::readNewLogEntries);
    connect(clearLogsButton, &QPushButton::clicked, [this]() { clearLogs(); });
    connect(updateTimer, &QTimer::timeout, this, &ProcessWindow::updateProcessInfo);
    connect(themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProcessWindow::onThemeChanged);
}

void ProcessWindow::setupConfigurationTab()
{
    configurationTab = new QWidget();
    auto* layout = new QVBoxLayout(configurationTab);

    QGroupBox* configGroup = new QGroupBox("Process Configuration");
    configGroup->setStyleSheet(GroupBoxStyle::primary());
    auto* configLayout = new QFormLayout(configGroup);
    configLayout->setLabelAlignment(Qt::AlignRight);

    nameLabel = new QLabel();
    commandLabel = new QLabel();
    workingDirLabel = new QLabel();

    configLayout->addRow("Name:", nameLabel);
    configLayout->addRow("Command:", commandLabel);
    configLayout->addRow("Working Directory:", workingDirLabel);

    layout->addWidget(configGroup);
    layout->addStretch();
}

void ProcessWindow::setProcess(const Process& process)
{
    currentProcess = process;
    setWindowTitle("Process Manager - " + process.getName());
    updateStatusDisplay();
}

void ProcessWindow::updateStatusDisplay()
{
    statusLabel->setText("Status: " + currentProcess.getStatusString());
    pidLabel->setText("PID: " + QString::number(currentProcess.getPID()));

    if (currentProcess.getStatus() == Process::Status::Running && currentProcess.getLastStartedAt().isValid())
    {
        qint64 uptimeSeconds = currentProcess.getLastStartedAt().secsTo(QDateTime::currentDateTime());
        uptimeLabel->setText("Uptime: " + QString::number(uptimeSeconds) + " seconds");
    }
    else
    {
        uptimeLabel->setText("Uptime: --");
    }

    nameLabel->setText(currentProcess.getName());
    commandLabel->setText(currentProcess.getCommand());
    workingDirLabel->setText(currentProcess.getWorkingDirectory());
}

void ProcessWindow::updateProcessInfo()
{
    updateStatusDisplay();
}

void ProcessWindow::readNewLogEntries()
{
    if (!logFile.isOpen())
    {
        return;
    }

    // Check if the user is currently at the bottom
    QScrollBar* scrollBar = logsTextEdit->verticalScrollBar();
    bool wasAtBottom = (scrollBar->value() == scrollBar->maximum());

    logFile.seek(lastLogPosition);

    QStringList newLines;
    while (!logFile.atEnd())
    {
        QString line = QString::fromUtf8(logFile.readLine());
        newLines << AnsiHtmlConverter::toHtml(line);
    }

    if (!newLines.isEmpty())
    {
        logsTextEdit->append(newLines.join(""));
    }

    lastLogPosition = logFile.pos();

    // Only auto scroll if the user was at the bottom
    if (wasAtBottom)
    {
        scrollBar->setValue(scrollBar->maximum());
    }
}

void ProcessWindow::onThemeChanged(int index)
{
    QString theme = themeComboBox->itemData(index).toString();
    applyTheme(theme);

    settings.setValue("terminal/theme", theme);
}

void ProcessWindow::applyTheme(const QString& themeName)
{
    QString style;

    if (themeName == "dark")
    {
        style = R"(
            QTextBrowser {
                background-color: #1e1e1e;
                color: #d4d4d4;
                border: 1px solid #3e3e3e;
            }
        )";
    }
    else if (themeName == "light")
    {
        style = R"(
            QTextBrowser {
                background-color: #ffffff;
                color: #000000;
                border: 1px solid #cccccc;
            }
        )";
    }
    else if (themeName == "terminal")
    {
        style = R"(
            QTextBrowser {
                background-color: #000000;
                color: #00ff00;
                border: 1px solid #00ff00;
            }
        )";
    }

    style += R"(
        QTextBrowser {
            border-radius: 4px;
            padding: 8px;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 9pt;
        }
    )";

    logsTextEdit->setStyleSheet(style);
}

void ProcessWindow::clearLogs()
{
    logsTextEdit->clear();

    if (logFile.isOpen())
    {
        logFile.close();
    }

    QFile file(logFile.fileName());
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        file.close();
    }
    else
    {
        QMessageBox::warning(this, "Clear Logs", "Failed to clear log file: " + file.fileName());
    }

    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        logsTextEdit->append("Failed to reopen log file after clearing.");
    }

    lastLogPosition = 0;
}
