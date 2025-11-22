#include "EmptyStateWidget.h"

#include "../../styles/ButtonStyle.h"
#include "../../styles/FontStyle.h"
#include <QPushButton>
#include <QVBoxLayout>

EmptyStateWidget::EmptyStateWidget(QWidget* parent) : QWidget(parent)
{
    setupUI();
    setupConnections();
}

void EmptyStateWidget::setupUI()
{
    QVBoxLayout* emptyStateLayout = new QVBoxLayout(this);
    emptyStateLayout->setAlignment(Qt::AlignCenter);

    emptyTitle = new QLabel("No project selected");
    emptyTitle->setStyleSheet(FontStyle::h1());
    emptyTitle->setAlignment(Qt::AlignCenter);

    emptySubtitle = new QLabel("Create a new project to get started.\n You can then add processes, run scripts, and open it in your IDE.");
    emptySubtitle->setStyleSheet(FontStyle::text());
    emptySubtitle->setAlignment(Qt::AlignCenter);

    createProjectButton = new QPushButton("Create Project");
    createProjectButton->setStyleSheet(ButtonStyle::primary());

    emptyStateLayout->addWidget(emptyTitle);
    emptyStateLayout->addWidget(emptySubtitle);
    emptyStateLayout->addSpacing(15);
    emptyStateLayout->addWidget(createProjectButton, 0, Qt::AlignCenter);
}

void EmptyStateWidget::setupConnections()
{
    connect(createProjectButton, &QPushButton::clicked, this, &EmptyStateWidget::createProjectClicked);
}

void EmptyStateWidget::refreshStyle()
{
    if (emptyTitle != nullptr)
    {
        emptyTitle->setStyleSheet(FontStyle::h1());
    }
    if (emptySubtitle != nullptr)
    {
        emptySubtitle->setStyleSheet(FontStyle::text());
    }
    if (createProjectButton != nullptr)
    {
        createProjectButton->setStyleSheet(ButtonStyle::primary());
    }
}
