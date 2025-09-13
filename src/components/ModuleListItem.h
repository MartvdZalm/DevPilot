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
    explicit ModuleListItem(const Module& module, const QString& iconEmoji, QWidget* parent = nullptr);
    QString cleanAnsi(const QString& text);
    void setLogs(const QString& text);
    void appendLog(const QString& line);
    void setModule(const Module& module);
    void startCommand();
    void stopCommand();

    QPushButton* getStartButton();
    QPushButton* getStopButton();
    QTextEdit* getLogs();

  private:
    QLabel* portLabel;
    QPushButton* startButton;
    QPushButton* stopButton;
    QTextEdit* logs;
    Module module;
    QProcess* process = nullptr;
};

#endif // MODULELISTITEM_H
