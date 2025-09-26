#include "MainWindow.h"
#include "HomeWindow.h"
#include "SettingsWindow.h"
#include "../events/AppEvents.h"
#include "../components/TitleBar.h"
#include <QVBoxLayout>

MainWindow::MainWindow(std::shared_ptr<RepositoryProvider> repoProvider, QWidget* parent)
    : QMainWindow(parent), repositoryProvider(std::move(repoProvider))
{
    QWidget* container = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    TitleBar* titleBar = new TitleBar(this);
    mainLayout->addWidget(titleBar);

    stackedWidget = new QStackedWidget(container);
    mainLayout->addWidget(stackedWidget);

    setCentralWidget(container);

    showHomePage();

    connect(&AppEvents::instance(), &AppEvents::navigateToSettings, this, &MainWindow::showSettingsPage);
}

void MainWindow::setPage(BaseWindow* newPage, bool addToHistory)
{
    if (!newPage)
    {
        return;
    }

    if (stackedWidget->count() > 0)
    {
        if (!addToHistory)
        {
            QWidget* oldPage = stackedWidget->currentWidget();
            stackedWidget->removeWidget(oldPage);
            oldPage->deleteLater();
        }
    }

    stackedWidget->addWidget(newPage);
    stackedWidget->setCurrentWidget(newPage);
}

void MainWindow::showHomePage()
{
    setPage(new HomeWindow(repositoryProvider, this), false);
}

void MainWindow::showSettingsPage()
{
    SettingsWindow* settingsWindow = new SettingsWindow(repositoryProvider);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->show();
}
