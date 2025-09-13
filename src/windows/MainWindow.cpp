#include "MainWindow.h"
#include "HomeWindow.h"
#include <QVBoxLayout>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    QWidget* container = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    stackedWidget = new QStackedWidget(container);

    mainLayout->addWidget(stackedWidget);

    setCentralWidget(container);

    showHomePage();
}

MainWindow::~MainWindow() {}

void MainWindow::setPage(BaseWindow* newPage, bool addToHistory)
{
    if (!newPage)
        return;

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
    setPage(new HomeWindow(this), false);
}
