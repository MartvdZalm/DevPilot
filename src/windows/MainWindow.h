#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../repositories/RepositoryProvider.h"
#include "BaseWindow.h"
#include <QMainWindow>
#include <QStack>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(RepositoryProvider& repositoryProvider, QWidget* parent = nullptr);

  private:
    void showHomePage();
    void showSettingsPage();
    void showSnippetsPage();
    void setPage(BaseWindow* newPage, bool addToHistory = true);

  private:
    RepositoryProvider& repositoryProvider;
    QStackedWidget* stackedWidget;
};

#endif // MAINWINDOW_H
