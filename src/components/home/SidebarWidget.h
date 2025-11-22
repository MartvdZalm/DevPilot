#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include "../../models/Project.h"
#include "../../repositories/RepositoryProvider.h"
#include "../../repositories/interfaces/IProjectRepository.h"
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
    void onAddProjectClicked();
    void refreshStyle();

  signals:
    void projectSelected(int projectId);

  private:
    void setupUI();
    void setupConnections();
    void onProjectItemClicked(QListWidgetItem* item);
    void refreshProjectList();
    void filterProjects(const QString& text);

    RepositoryProvider& repoProvider;
    IProjectRepository& projectRepository;
    QListWidget* projectList = nullptr;
    QPushButton* addProjectButton = nullptr;
    QLineEdit* searchInput = nullptr;
    QListWidgetItem* currentlySelectedItem = nullptr;
    QString currentSortType;
};

#endif // SIDEBARWIDGET_H
