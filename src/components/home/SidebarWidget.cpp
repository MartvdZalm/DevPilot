#include "SidebarWidget.h"

#include "../../core/Logger.h"
#include "../../core/IconManager.h"
#include "../../styles/ButtonStyle.h"
#include "../../styles/ListStyle.h"
#include "../../styles/InputStyle.h"
#include "../dialogs/ProjectDialog.h"
#include "../../events/AppEvents.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <qmenu.h>

SidebarWidget::SidebarWidget(RepositoryProvider& repoProvider, QWidget* parent)
    : QWidget(parent), repoProvider(repoProvider), projectRepository(repoProvider.getProjectRepository())
{
    setupUI();
    setupConnections();
}

void SidebarWidget::setupUI()
{
    setFixedWidth(250);

    QFrame* mainFrame = new QFrame(this);
    mainFrame->setStyleSheet("QFrame { border-right: 1px solid #3a3f47; background-color: transparent; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(mainFrame);

    QVBoxLayout* sidebarLayout = new QVBoxLayout(mainFrame);
    sidebarLayout->setContentsMargins(15, 15, 15, 15);
    sidebarLayout->setSpacing(10);

    QHBoxLayout* projectsHeaderLayout = new QHBoxLayout();

    QLabel* projectsLabel = new QLabel("Projects");
    projectsLabel->setStyleSheet("font-size: 18px; font-weight: bold; border: none;");

    addProjectButton = new QPushButton("", this);
    addProjectButton->setIcon(IconManager::instance().add());
    addProjectButton->setStyleSheet(ButtonStyle::icon());
    addProjectButton->setIconSize(QSize(25, 25));

    projectsHeaderLayout->addWidget(projectsLabel);
    projectsHeaderLayout->addStretch();
    projectsHeaderLayout->addWidget(addProjectButton);
    sidebarLayout->addLayout(projectsHeaderLayout);

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search...");
    searchInput->setStyleSheet(InputStyle::primary());
    sidebarLayout->addWidget(searchInput);

    projectList = new QListWidget();
    projectList->setStyleSheet(ListStyle::primary());
    projectList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    projectList->setTextElideMode(Qt::ElideRight);
    sidebarLayout->addWidget(projectList);

    sidebarLayout->addWidget(projectList);

    sidebarLayout->addStretch();
}

void SidebarWidget::setupConnections()
{
    connect(addProjectButton, &QPushButton::clicked, this, &SidebarWidget::onAddProjectClicked);
    connect(projectList, &QListWidget::itemClicked, this, &SidebarWidget::onProjectItemClicked);
    connect(searchInput, &QLineEdit::textChanged, this, &SidebarWidget::filterProjects);

    connect(&AppEvents::instance(), &AppEvents::refreshHomeSidebar, this, &SidebarWidget::refreshProjectList);
}

void SidebarWidget::setProjects(const QList<Project>& projects)
{
    projectList->clear();
    currentlySelectedItem = nullptr;

    for (const auto& project : projects)
    {
        auto* item = new QListWidgetItem(QIcon(":/Images/Folder"), project.getName());
        item->setData(Qt::UserRole, project.getId());
        projectList->addItem(item);
    }
}

void SidebarWidget::onAddProjectClicked()
{
    ProjectDialog dialog(repoProvider, this);
    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    std::optional<Project> savedProject = projectRepository.save(dialog.getProject());
    if (!savedProject.has_value())
    {
        return;
    }

    LOG_INFO("Created project: " + savedProject->getName());
    refreshProjectList();
}

void SidebarWidget::onProjectItemClicked(QListWidgetItem* item)
{
    if (!item)
        return;

    if (currentlySelectedItem && currentlySelectedItem != item)
    {
        currentlySelectedItem->setIcon(QIcon(":/Images/Folder"));
    }

    currentlySelectedItem = item;
    item->setIcon(QIcon(":/Images/FolderOpen"));

    int projectId = item->data(Qt::UserRole).toInt();

    projectRepository.updateLastOpened(projectId);

    emit projectSelected(projectId);
}

void SidebarWidget::setSelectedProject(int projectId)
{
    clearSelection();

    for (int i = 0; i < projectList->count(); ++i)
    {
        QListWidgetItem* item = projectList->item(i);
        if (item->data(Qt::UserRole).toInt() == projectId)
        {
            currentlySelectedItem = item;
            item->setIcon(QIcon(":/Images/FolderOpen"));
            projectList->setCurrentItem(item);
            break;
        }
    }
}

void SidebarWidget::clearSelection()
{
    if (currentlySelectedItem)
    {
        currentlySelectedItem->setIcon(QIcon(":/Images/Folder"));
        currentlySelectedItem = nullptr;
    }
    projectList->clearSelection();
}

void SidebarWidget::refreshProjectList()
{
    int currentProjectId = -1;
    if (currentlySelectedItem)
    {
        currentProjectId = currentlySelectedItem->data(Qt::UserRole).toInt();
    }

    auto projects = projectRepository.findAllByRecentlyOpened();
    setProjects(projects);

    if (currentProjectId != -1)
    {
        setSelectedProject(currentProjectId);
    }
}

void SidebarWidget::filterProjects(const QString& text)
{
    if (text.isEmpty())
    {
        refreshProjectList();
        return;
    }

    for (int i = 0; i < projectList->count(); ++i)
    {
        QListWidgetItem* item = projectList->item(i);
        bool matches = item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

void SidebarWidget::refreshStyle()
{
    addProjectButton->setIcon(IconManager::instance().add());
}
