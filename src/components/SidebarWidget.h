#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include "../models/Project.h"
#include "../repositories/interfaces/IProjectRepository.h"
#include "../repositories/RepositoryProvider.h"
#include <QList>
#include <QListWidget>
#include <QPushButton>

/**
 * Sidebar used on the HomeWindow to display the list of projects.
 */
class SidebarWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit SidebarWidget(RepositoryProvider& repoProvider, QWidget* parent = nullptr);

    void setProjects(const QList<Project>& projects);
    void setSelectedProject(int projectId);
    void clearSelection();

  signals:
    void projectSelected(int projectId);

  private:
    void setupUI();
    void setupConnections();
    void onProjectItemClicked(QListWidgetItem* item);
    void onAddProjectClicked();
    void refreshProjectList();

    RepositoryProvider& repoProvider;
    IProjectRepository& projectRepository;
    QListWidget* projectList = nullptr;
    QPushButton* addProjectButton = nullptr;
    QListWidgetItem* currentlySelectedItem = nullptr;
};

#endif // SIDEBARWIDGET_H
