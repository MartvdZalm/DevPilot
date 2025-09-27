#include "EmptyStateWidget.h"

#include "../styles/ButtonStyle.h"
#include "../windows/HomeWindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

EmptyStateWidget::EmptyStateWidget(QWidget* parent) : QWidget(parent)
{
    setupUI();
}

void EmptyStateWidget::setupUI()
{
    QVBoxLayout* emptyStateLayout = new QVBoxLayout(this);
    emptyStateLayout->setAlignment(Qt::AlignCenter);

    QLabel* emptyTitle = new QLabel("No project selected");
    emptyTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #aaaaaa;");
    emptyTitle->setAlignment(Qt::AlignCenter);
    QLabel* emptySubtitle = new QLabel("Create a new project to get started.\n"
                                       "You can then add modules, run scripts, and open it in your IDE.");
    emptySubtitle->setStyleSheet("font-size: 14px; color: #888888;");
    emptySubtitle->setAlignment(Qt::AlignCenter);

    QPushButton* createProjectBtn = new QPushButton("Create Project");
    createProjectBtn->setStyleSheet(ButtonStyle::primary());
    // connect(createProjectBtn, &QPushButton::clicked, this, &HomeWindow::onAddProjectClicked);

    emptyStateLayout->addWidget(emptyTitle);
    emptyStateLayout->addWidget(emptySubtitle);
    emptyStateLayout->addSpacing(15);
    emptyStateLayout->addWidget(createProjectBtn, 0, Qt::AlignCenter);
}
