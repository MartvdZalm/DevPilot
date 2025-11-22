#include "HomeWindow.h"

HomeWindow::HomeWindow(RepositoryProvider& repoProvider, QWidget* parent)
    : BaseWindow(parent), repositoryProvider(repoProvider), projectRepository(repoProvider.getProjectRepository())
{
    setupUI();
    setupConnections();
}

void HomeWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QHBoxLayout* pageContentLayout = new QHBoxLayout();
    pageContentLayout->setContentsMargins(0, 0, 0, 0);
    pageContentLayout->setSpacing(0);

    titleBar = new TitleBar(this);
    mainLayout->addWidget(titleBar);

    // Sidebar shows project list
    sidebarWidget = new SidebarWidget(repositoryProvider, this);
    sidebarWidget->setProjects(projectRepository.findAll());
    pageContentLayout->addWidget(sidebarWidget);

    // Main content area (right side)
    QWidget* mainContentWidget = new QWidget();
    QVBoxLayout* mainContentLayout = new QVBoxLayout(mainContentWidget);
    mainContentLayout->setContentsMargins(20, 20, 20, 20);
    mainContentLayout->setSpacing(15);

    // Empty state shown when no project is selected
    emptyStateWidget = new EmptyStateWidget(this);
    connect(emptyStateWidget, &EmptyStateWidget::createProjectClicked, sidebarWidget, &SidebarWidget::onAddProjectClicked);
    emptyStateWidget->show();

    mainContentLayout->addWidget(emptyStateWidget);

    // Project details shown when a project is selected
    projectDetailsWidget = new ProjectDetailsWidget(repositoryProvider, this);
    projectDetailsWidget->hide();
    mainContentLayout->addWidget(projectDetailsWidget);

    pageContentLayout->addWidget(mainContentWidget);

    mainLayout->addLayout(pageContentLayout);
}

void HomeWindow::setupConnections()
{
    connect(sidebarWidget, &SidebarWidget::projectSelected, this, &HomeWindow::onProjectSelected);
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, &HomeWindow::applyTheme);
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

void HomeWindow::applyTheme()
{
    titleBar->refreshStyle();
    sidebarWidget->refreshStyle();
    emptyStateWidget->refreshStyle();
}
