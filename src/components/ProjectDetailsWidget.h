#ifndef PROJECTDETAILSWIDGET_H
#define PROJECTDETAILSWIDGET_H

#include "../models/Note.h"
#include "../models/Project.h"
#include "../repositories/RepositoryProvider.h"
#include "../repositories/interfaces/IEditorRepository.h"
#include "../repositories/interfaces/INoteRepository.h"
#include "../repositories/interfaces/IProcessRepository.h"
#include "../repositories/interfaces/IProjectRepository.h"
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>
#include <QSettings>

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
    void loadProjectProcesses(int projectId);
    void loadProjectNotes(int projectId);

    void refreshProject();
    void refreshProcesses();

    void onAddNoteClicked();
    void onOpenNoteDialog(const Note& note);
    void onEditProjectClicked();
    void onOpenInFolderClicked();
    void onOpenInTerminalClicked();
    void onOpenInIDEClicked();
    void onOpenAllAppsClicked();
    void onAddProcessClicked();
    void onToggleNotesClicked(bool checked);
    void onEditProcessClicked(const Process& process);
    void onDeleteProcessClicked(const Process& process);

  private:
    RepositoryProvider& repositoryProvider;
    IProjectRepository& projectRepository;
    IProcessRepository& processRepository;
    INoteRepository& noteRepository;
    IEditorRepository& editorRepository;

    Project currentProject;
    QList<Process> currentProcesses;

    QLabel* projectNameLabel = nullptr;
    QLabel* projectPathLabel = nullptr;
    QPushButton* editProjectButton = nullptr;

    QPushButton* openInFolderButton = nullptr;
    QPushButton* openInTerminalButton = nullptr;
    QPushButton* openInIDEButton = nullptr;
    QPushButton* openAllAppsButton;

    QGridLayout* processesLayout = nullptr;
    QGridLayout* notesLayout = nullptr;
    QScrollArea* processesScrollArea = nullptr;
    QScrollArea* notesScrollArea = nullptr;
    QPushButton* addProcessButton = nullptr;
    QPushButton* startAllProcessesButton = nullptr;
    QPushButton* stopAllProcessesButton = nullptr;
    QVBoxLayout* processListLayout = nullptr;
    QToolButton* addNoteButton = nullptr;
    QWidget* notesContainer = nullptr;
    QLayout* notesListLayout = nullptr;
    QToolButton* toggleNotesBtn = nullptr;
    QIcon arrowDown, arrowRight;
    QSettings settings;
};

#endif // PROJECTDETAILSWIDGET_H
