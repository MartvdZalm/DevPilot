#ifndef PROCESSLISTITEM_H
#define PROCESSLISTITEM_H

#include "../../models/Process.h"
#include "../../repositories/interfaces/IProcessRepository.h"
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>

class ProcessListItem : public QGroupBox
{
    Q_OBJECT

  public:
    explicit ProcessListItem(Process& process, IProcessRepository& processRepository, QWidget* parent = nullptr);
    ~ProcessListItem();

    void setProcess(Process& process);
    QPushButton* getStartButton();
    QPushButton* getStopButton();
    Process getProcess();

  private:
    void setupUI();
    void setupConnections();
    void startCommand();
    void stopCommand();
    void updateStatus();
    void openTerminalWindow();
    bool isProcessRunning(int pid);
    int findPidByPort(int port);
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void startBackgroundMonitoring();
    void startPortPolling();
    void handleProcessStarted();
    void prepareForShutdown();

  signals:
    void editRequested(Process& process);
    void deleteRequested(Process& process);

  private:
    IProcessRepository& processRepository;

    Process process;
    QLabel* portLabel = nullptr;
    QLabel* statusLabel = nullptr;
    QPushButton* startButton = nullptr;
    QPushButton* stopButton = nullptr;
    QPushButton* terminalButton = nullptr;
    QPushButton* editButton = nullptr;
    QPushButton* deleteButton = nullptr;
    QProcess* qProcess = nullptr;
    QTimer* pollTimer = nullptr;
    QTimer* monitorTimer = nullptr;
    bool isShuttingDown = false;
};

#endif // PROCESSLISTITEM_H
