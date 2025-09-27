#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "../components/EmptyStateWidget.h"
#include "../components/ProjectDetailsWidget.h"
#include "../components/SidebarWidget.h"
#include "../repositories/IProjectRepository.h"
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
    void onProjectSelected(int projectId);

  protected:
    RepositoryProvider& repositoryProvider;
    IProjectRepository& projectRepository;

    SidebarWidget* sidebarWidget = nullptr;
    EmptyStateWidget* emptyStateWidget = nullptr;
    ProjectDetailsWidget* projectDetailsWidget = nullptr;

    QHBoxLayout* mainLayout = nullptr;
};

#endif // HOMEWINDOW_H
