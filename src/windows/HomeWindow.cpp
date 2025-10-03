#include "HomeWindow.h"

HomeWindow::HomeWindow(RepositoryProvider& repoProvider, QWidget* parent)
    : BaseWindow(parent), repositoryProvider(repoProvider), projectRepository(repoProvider.getProjectRepository())
{
    setupUI();
    setupConnections();
}

void HomeWindow::setupUI()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Sidebar shows project list
    sidebarWidget = new SidebarWidget(repositoryProvider, this);
    sidebarWidget->setProjects(projectRepository.findAll());
    mainLayout->addWidget(sidebarWidget);

    // Main content area (right side)
    QWidget* mainContentWidget = new QWidget();
    QVBoxLayout* mainContentLayout = new QVBoxLayout(mainContentWidget);
    mainContentLayout->setContentsMargins(20, 20, 20, 20);
    mainContentLayout->setSpacing(15);

    // Empty state shown when no project is selected
    emptyStateWidget = new EmptyStateWidget(this);
    emptyStateWidget->show();

    mainContentLayout->addWidget(emptyStateWidget);

    // Project details shown when a project is selected
    projectDetailsWidget = new ProjectDetailsWidget(repositoryProvider, this);
    projectDetailsWidget->hide();
    mainContentLayout->addWidget(projectDetailsWidget);

    mainLayout->addWidget(mainContentWidget);
}

void HomeWindow::setupConnections()
{
    connect(sidebarWidget, &SidebarWidget::projectSelected, this, &HomeWindow::onProjectSelected);
}

void HomeWindow::onProjectSelected(int projectId)
{
    std::optional<Project> project = projectRepository.findById(projectId);

    if (!project.has_value())
    {
        return;
    }

    Project& currentProject = projectDetailsWidget->getProject();

    if (currentProject.getId() == projectId)
    {
        sidebarWidget->clearSelection();
        projectDetailsWidget->setProject(Project());
        projectDetailsWidget->hide();
        emptyStateWidget->show();
    }
    else
    {
        sidebarWidget->setSelectedProject(projectId);
        projectDetailsWidget->setProject(project.value());
        projectDetailsWidget->show();
        emptyStateWidget->hide();
    }
}
