#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "../components/home/EmptyStateWidget.h"
#include "../components/home/ProjectDetailsWidget.h"
#include "../components/home/SidebarWidget.h"
#include "../components/home/TitleBar.h"
#include "../repositories/interfaces/IProjectRepository.h"
#include "../repositories/RepositoryProvider.h"
#include "BaseWindow.h"

class HomeWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit HomeWindow(RepositoryProvider& repositoryProvider, QWidget* parent = nullptr);

  private:
    void setupUI() override;
    void setupConnections() override;
    void applyTheme() override;

    void onProjectSelected(int projectId);

  protected:
    RepositoryProvider& repositoryProvider;
    IProjectRepository& projectRepository;

    SidebarWidget* sidebarWidget = nullptr;
    EmptyStateWidget* emptyStateWidget = nullptr;
    ProjectDetailsWidget* projectDetailsWidget = nullptr;
    TitleBar* titleBar = nullptr;
};

#endif // HOMEWINDOW_H
