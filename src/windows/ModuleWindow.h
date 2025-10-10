#ifndef MODULEWINDOW_H
#define MODULEWINDOW_H

#include "../models/Module.h"
#include "BaseWindow.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QCheckBox>

class ModuleWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit ModuleWindow(QWidget* parent = nullptr, const Module& module = Module());

  private:
    void setupUI() override;
    void setupConnections() override;
    void updateControlsState(bool running);
    void updateStatusIndicator(bool running);
    void appendColoredText(const QString& text, const QString& color);
    void autoScroll();
    void restartProcess();
    void onReadyReadError();
    void onReadyRead();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onCommandEntered();
    void clearTerminal();
    void startProcess();
    void stopProcess();

    QWidget* createHeader();
    QHBoxLayout* createControls();
    QTabWidget* createTabWidget();
    QWidget* createInputSection();

  private:
    const Module& module;
    QProcess* process;
    QTextEdit* terminalOutput;
    QLineEdit* commandInput;
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* clearButton;
    QPushButton* restartButton;
    QPushButton* sendButton;
    QCheckBox* autoScrollCheckbox;
    QLabel* statusIndicator;
    QTabWidget* tabWidget;
};

#endif // MODULEWINDOW_H