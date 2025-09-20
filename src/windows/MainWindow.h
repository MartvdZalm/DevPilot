#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BaseWindow.h"
#include <QMainWindow>
#include <QStack>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private:
    void showHomePage();
    void showSettingsPage();
    void setPage(BaseWindow* newPage, bool addToHistory = true);

    QStackedWidget* stackedWidget;
};

#endif // MAINWINDOW_H
