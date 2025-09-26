#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "../models/Module.h"
#include "../models/Project.h"
#include "../models/Note.h"
#include "../repositories/INoteRepository.h"
#include "../repositories/IModuleRepository.h"
#include "../repositories/IProjectRepository.h"
#include "../repositories/RepositoryProvider.h"
#include "BaseWindow.h"
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QToolButton>

class HomeWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit HomeWindow(std::shared_ptr<RepositoryProvider> repositoryProvider, QWidget* parent = nullptr);

  private slots:
    void onAddProjectClicked();
    void onEditProjectClicked();
    void onOpenInFolderClicked();
    void onOpenInTerminalClicked();
    void onOpenInIDEClicked();
    void onProjectSelected(QListWidgetItem* item);
    void onModuleStartClicked(int moduleId);
    void onModuleStopClicked(int moduleId);

  private:
    void setupUI() override;
    void setupConnections() override;
    void loadProjects();
    void loadProjectModules(int projectId);
    void loadProjectNotes(int projectId);
    void refreshProjectList();
    void refreshModuleDisplay();
    void onAddNoteClicked();
    void openNoteDialog(const Note& note);
    QString statusIconForModule(const Module& m) const;

  protected:
    std::shared_ptr<RepositoryProvider> repositoryProvider;
    std::shared_ptr<IProjectRepository> projectRepository;
    std::shared_ptr<IModuleRepository> moduleRepository;
    std::shared_ptr<INoteRepository> noteRepository;
    Project currentProject;
    QList<Module> currentModules;
    QListWidget* projectList = nullptr;
    QListWidgetItem* openedProjectItem = nullptr;
    QLabel* projectPathLabel = nullptr;
    QLabel* projectNameLabel = nullptr;
    QPushButton* addProjectButton = nullptr;
    QPushButton* addModuleButton = nullptr;
    QVBoxLayout* moduleListLayout = nullptr;
    QScrollArea* notesScrollArea = nullptr;
    QToolButton* addNoteButton = nullptr;
    QWidget* notesContainer = nullptr;
    QLayout* notesListLayout = nullptr;
    QWidget* emptyStateWidget = nullptr;
    QWidget* projectDetailsWidget = nullptr;
};

#endif // HOMEWINDOW_H
