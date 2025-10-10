#ifndef MODULELISTITEM_H
#define MODULELISTITEM_H

#include "../models/Module.h"
#include <QGroupBox>
#include <QLabel>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>

class ModuleListItem : public QGroupBox
{
    Q_OBJECT

  public:
    explicit ModuleListItem(const Module& module, QWidget* parent = nullptr);

    void setLogs(const QString& text);
    void setModule(const Module& module);
    QTextEdit* getLogs();
    QPushButton* getStartButton();
    QPushButton* getStopButton();
    Module getModule() const;

  private:
    QString cleanAnsi(const QString& text);
    void appendLog(const QString& line);
    void startCommand();
    void stopCommand();
    void updateStatus();
    void openTerminalWindow();

signals:
    void editRequested(const Module& module);
    void deleteRequested(const Module& module);

  private:
    Module module;
    QLabel* portLabel = nullptr;
    QLabel* statusLabel = nullptr;
    QPushButton* startButton = nullptr;
    QPushButton* stopButton = nullptr;
    QPushButton* terminalButton = nullptr;
    QPushButton* editButton = nullptr;
    QPushButton* deleteButton = nullptr;
    QTextEdit* logs = nullptr;
    QProcess* process = nullptr;
};

#endif // MODULELISTITEM_H
