#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BaseWindow.h"
#include "../repositories/RepositoryProvider.h"
#include <QMainWindow>
#include <QStack>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(std::shared_ptr<RepositoryProvider> repositoryProvider, QWidget* parent = nullptr);

  private:
    void showHomePage();
    void showSettingsPage();
    void setPage(BaseWindow* newPage, bool addToHistory = true);

  private:
    std::shared_ptr<RepositoryProvider> repositoryProvider;
    QStackedWidget* stackedWidget;
};

#endif // MAINWINDOW_H
