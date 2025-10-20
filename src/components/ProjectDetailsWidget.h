#ifndef PROJECTDETAILSWIDGET_H
#define PROJECTDETAILSWIDGET_H

#include "../models/Note.h"
#include "../models/Project.h"
#include "../repositories/interfaces/IModuleRepository.h"
#include "../repositories/interfaces/INoteRepository.h"
#include "../repositories/interfaces/IProjectRepository.h"
#include "../repositories/RepositoryProvider.h"
#include "../repositories/interfaces/IEditorRepository.h"
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

class ProjectDetailsWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit ProjectDetailsWidget(RepositoryProvider& repoProvider, QWidget* parent = nullptr);

    void setProject(Project project);
    Project& getProject();

  private:
    void setupUI();
    void setupConnections();
    QHBoxLayout* createHeader();

    void loadProject();
    void loadProjectModules(int projectId);
    void loadProjectNotes(int projectId);

    void refreshProject();
    void refreshModules();

    void onAddNoteClicked();
    void onOpenNoteDialog(const Note& note);
    void onEditProjectClicked();
    void onOpenInFolderClicked();
    void onOpenInTerminalClicked();
    void onOpenInIDEClicked();
    void onAddModuleClicked();
    void onToggleNotesClicked(bool checked);
    void onEditModuleClicked(const Module& module);
    void onDeleteModuleClicked(const Module& module);

  private:
    RepositoryProvider& repositoryProvider;
    IProjectRepository& projectRepository;
    IModuleRepository& moduleRepository;
    INoteRepository& noteRepository;
    IEditorRepository& editorRepository;

    Project currentProject;
    QList<Module> currentModules;

    QLabel* projectNameLabel = nullptr;
    QLabel* projectPathLabel = nullptr;
    QPushButton* editProjectButton = nullptr;
    QPushButton* openInFolderButton = nullptr;
    QPushButton* openInTerminalButton = nullptr;
    QPushButton* openInIDEButton = nullptr;
    QGridLayout* modulesLayout = nullptr;
    QGridLayout* notesLayout = nullptr;
    QScrollArea* modulesScrollArea = nullptr;
    QScrollArea* notesScrollArea = nullptr;
    QPushButton* addModuleButton = nullptr;
    QPushButton* startAllModulesButton = nullptr;
    QPushButton* stopAllModulesButton = nullptr;
    QVBoxLayout* moduleListLayout = nullptr;
    QToolButton* addNoteButton = nullptr;
    QWidget* notesContainer = nullptr;
    QLayout* notesListLayout = nullptr;
    QToolButton* toggleNotesBtn = nullptr;
    QIcon arrowDown, arrowRight;
};

#endif // PROJECTDETAILSWIDGET_H
