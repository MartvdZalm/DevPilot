#ifndef PROCESSWINDOW_H
#define PROCESSWINDOW_H

#include "../models/Process.h"
#include "BaseWindow.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QProcess>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QTabWidget>
#include <QTableWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

class ProcessWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit ProcessWindow(const Process& process, QWidget* parent = nullptr);
    void setProcess(const Process& process);

  private slots:
    void updateProcessInfo();

  private:
    void setupUI();
    void setupConnections();
    void updateStatusDisplay();
    void setupLogsTab();
    void setupConfigurationTab();
    void readNewLogEntries();
    void onThemeChanged(int index);
    void applyTheme(const QString& themeName);
    void clearLogs();

  private:
    Process currentProcess;
    QTimer* updateTimer;
    QLabel* statusLabel;
    QLabel* pidLabel;
    QLabel* uptimeLabel;
    QTabWidget* tabWidget;
    QWidget* logsTab;
    QWidget* configurationTab;
    QPushButton* clearLogsButton;
    QTextBrowser* logsTextEdit;
    QFile logFile;
    QTimer* logUpdateTimer;
    qint64 lastLogPosition = 0;
    QLabel* nameLabel;
    QLabel* commandLabel;
    QLabel* workingDirLabel;
    QComboBox* themeComboBox;
    QSettings settings;
};

#endif // PROCESSWINDOW_H
