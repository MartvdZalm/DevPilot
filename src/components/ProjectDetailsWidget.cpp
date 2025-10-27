#include "ProjectDetailsWidget.h"

#include "../core/Logger.h"
#include "../core/ProjectLauncher.h"
#include "../styles/ButtonStyle.h"
#include "../styles/FontStyle.h"
#include "FlowLayout.h"
#include "NoteCard.h"
#include "ProcessListItem.h"
#include "dialogs/ChooseEditorDialog.h"
#include "dialogs/NoteDialog.h"
#include "dialogs/ProcessDialog.h"
#include "dialogs/ProjectDialog.h"
#include <QDesktopServices>
#include <QDir>
#include <QListWidget>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>

ProjectDetailsWidget::ProjectDetailsWidget(RepositoryProvider& repoProvider, QWidget* parent)
    : QWidget(parent), repositoryProvider(repoProvider), projectRepository(repoProvider.getProjectRepository()),
      processRepository(repoProvider.getProcessRepository()), noteRepository(repoProvider.getNoteRepository()),
      editorRepository(repoProvider.getEditorRepository()), settings("Dev", "Pilot")
{
    setupUI();
    setupConnections();
}

void ProjectDetailsWidget::setupUI()
{
    QVBoxLayout* projectDetailsLayout = new QVBoxLayout(this);
    projectDetailsLayout->setContentsMargins(0, 0, 0, 0);
    projectDetailsLayout->setSpacing(15);

    projectDetailsLayout->addLayout(createHeader());

    // Processes section header with toggle button
    QHBoxLayout* processControlsLayout = new QHBoxLayout();
    processControlsLayout->setContentsMargins(0, 10, 0, 10);

    QLabel* processTitle = new QLabel("Processes");
    processTitle->setStyleSheet(FontStyle::h2());
    processControlsLayout->addWidget(processTitle);

    projectDetailsLayout->addLayout(processControlsLayout);

    // Scrollable area for processes
    processesScrollArea = new QScrollArea();
    processesScrollArea->setWidgetResizable(true);
    processesScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    processesScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* processesContainer = new QWidget();
    processesContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    processListLayout = new QVBoxLayout(processesContainer);
    processListLayout->setContentsMargins(0, 0, 0, 0);
    processListLayout->setSpacing(15);
    processListLayout->setAlignment(Qt::AlignTop);

    processesScrollArea->setWidget(processesContainer);

    projectDetailsLayout->addWidget(processesScrollArea, 1);

    toggleNotesBtn = new QToolButton();
    toggleNotesBtn->setStyleSheet(ButtonStyle::icon());
    toggleNotesBtn->setIconSize(QSize(24, 24));
    toggleNotesBtn->setText("Notes");
    toggleNotesBtn->setCheckable(true);
    toggleNotesBtn->setChecked(true);
    toggleNotesBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    arrowDown.addFile(":/Images/ArrowDropDown");
    arrowRight.addFile(":/Images/ArrowDropUp");
    toggleNotesBtn->setIcon(arrowDown);

    notesScrollArea = new QScrollArea();
    notesScrollArea->setMaximumHeight(160);
    notesScrollArea->setWidgetResizable(true);
    notesScrollArea->setFrameShape(QFrame::NoFrame);

    bool notesVisible = settings.value("NotesVisible", true).toBool();

    toggleNotesBtn->setChecked(notesVisible);
    notesScrollArea->setVisible(notesVisible);

    if (notesVisible)
        toggleNotesBtn->setIcon(arrowDown);
    else
        toggleNotesBtn->setIcon(arrowRight);

    notesContainer = new QWidget();
    auto* flow = new FlowLayout(notesContainer, 0, 12, 12);
    notesListLayout = flow;
    notesScrollArea->setWidget(notesContainer);

    addNoteButton = new QToolButton(notesContainer);
    addNoteButton->setFixedSize(170, 150);
    addNoteButton->setText("Add Note");
    addNoteButton->setCursor(Qt::PointingHandCursor);
    addNoteButton->setStyleSheet(R"(
        QToolButton {
          background: #E3F2FD;
          border: 2px dashed #90CAF9;
          border-radius: 8px;
          color: #1565C0;
          font-size: 12px;
          font-weight: bold;
        }
        QToolButton:hover {
          background: #BBDEFB;
        }
    )");

    notesListLayout->addWidget(addNoteButton);

    projectDetailsLayout->addWidget(toggleNotesBtn);
    projectDetailsLayout->addWidget(notesScrollArea);
}

void ProjectDetailsWidget::setupConnections()
{
    connect(editProjectButton, &QPushButton::clicked, this, &ProjectDetailsWidget::onEditProjectClicked);
    connect(openInFolderButton, &QPushButton::clicked, this, &ProjectDetailsWidget::onOpenInFolderClicked);
    connect(openInTerminalButton, &QPushButton::clicked, this, &ProjectDetailsWidget::onOpenInTerminalClicked);
    connect(openInIDEButton, &QPushButton::clicked, this, &ProjectDetailsWidget::onOpenInIDEClicked);
    connect(openAllAppsButton, &QPushButton::clicked, this, &ProjectDetailsWidget::onOpenAllAppsClicked);
    connect(addProcessButton, &QPushButton::clicked, this, &ProjectDetailsWidget::onAddProcessClicked);
    connect(toggleNotesBtn, &QToolButton::toggled, this, &ProjectDetailsWidget::onToggleNotesClicked);
    connect(addNoteButton, &QToolButton::clicked, this, &ProjectDetailsWidget::onAddNoteClicked);
}

QHBoxLayout* ProjectDetailsWidget::createHeader()
{
    QHBoxLayout* projectHeaderLayout = new QHBoxLayout();

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(8);

    projectNameLabel = new QLabel();
    projectNameLabel->setStyleSheet(FontStyle::h1());

    editProjectButton = new QPushButton(QIcon(":/Images/Edit"), "");
    editProjectButton->setStyleSheet(ButtonStyle::icon());
    editProjectButton->setIconSize(QSize(20, 20));

    titleLayout->addWidget(projectNameLabel);
    titleLayout->addWidget(editProjectButton);
    titleLayout->addStretch();

    projectPathLabel = new QLabel();
    projectPathLabel->setStyleSheet(FontStyle::text());

    addProcessButton = new QPushButton("+");
    addProcessButton->setFixedSize(25, 25);
    addProcessButton->setStyleSheet(ButtonStyle::primary());

    QVBoxLayout* projectInfoLayout = new QVBoxLayout();
    projectInfoLayout->addLayout(titleLayout);
    projectInfoLayout->addWidget(projectPathLabel);
    projectInfoLayout->addWidget(addProcessButton);

    openInFolderButton = new QPushButton(QIcon(":/Images/Folder"), "");
    openInFolderButton->setStyleSheet(ButtonStyle::primary());

    openInTerminalButton = new QPushButton(QIcon(":/Images/Terminal"), "");
    openInTerminalButton->setStyleSheet(ButtonStyle::primary());

    openInIDEButton = new QPushButton(QIcon(":/Images/Code"), "");
    openInIDEButton->setStyleSheet(ButtonStyle::primary());

    openAllAppsButton = new QPushButton(QIcon(":/Images/SDK"), "");
    openAllAppsButton->setStyleSheet(ButtonStyle::primary());

    projectHeaderLayout->addLayout(projectInfoLayout);
    projectHeaderLayout->addStretch();
    projectHeaderLayout->addWidget(openInFolderButton);
    projectHeaderLayout->addWidget(openInTerminalButton);
    projectHeaderLayout->addWidget(openInIDEButton);
    projectHeaderLayout->addWidget(openAllAppsButton);

    return projectHeaderLayout;
}

void ProjectDetailsWidget::setProject(Project project)
{
    this->currentProject = project;
    refreshProject();
    loadProject();
    show();
}

Project& ProjectDetailsWidget::getProject()
{
    return currentProject;
}

void ProjectDetailsWidget::loadProject()
{
    int projectId = currentProject.getId();

    loadProjectProcesses(projectId);
    loadProjectNotes(projectId);
}

void ProjectDetailsWidget::loadProjectProcesses(int projectId)
{
    currentProcesses = processRepository.findByProjectId(projectId);
    refreshProcesses();

    LOG_INFO("Loaded " + QString::number(currentProcesses.size()) +
             " processes for project ID: " + QString::number(projectId));
}

void ProjectDetailsWidget::loadProjectNotes(int projectId)
{
    QList<Note> currentNotes = noteRepository.findByProjectId(projectId);

    // Clear existing notes (preserving the add note button)
    for (int i = notesListLayout->count() - 1; i >= 0; --i)
    {
        QLayoutItem* item = notesListLayout->itemAt(i);
        if (!item)
            continue;

        QWidget* w = item->widget();
        if (w && w == addNoteButton)
            continue;

        QLayoutItem* removed = notesListLayout->takeAt(i);
        if (removed)
        {
            if (removed->widget())
                removed->widget()->deleteLater();
            delete removed;
        }
    }

    // Add note cards for each note
    for (const Note& note : currentNotes)
    {
        NoteCard* card = new NoteCard(note, notesContainer);

        connect(card, &NoteCard::clicked, this, [this](const Note& n) { onOpenNoteDialog(n); });

        connect(card, &NoteCard::deleteClicked, this,
                [this](const Note& n)
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Delete Note",
                                                  QString("Are you sure you want to delete '%1'?").arg(n.getTitle()),
                                                  QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        noteRepository.deleteById(n.getId());
                        loadProjectNotes(currentProject.getId());
                    }
                });

        notesListLayout->addWidget(card);
    }
}

void ProjectDetailsWidget::refreshProject()
{
    projectNameLabel->setText(currentProject.getName());

    // Enable/disable action buttons based on whether path is set
    if (!currentProject.getDirectoryPath().isEmpty())
    {
        projectPathLabel->setText(QString("Directory  %1").arg(currentProject.getDirectoryPath()));

        openInFolderButton->setEnabled(true);
        openInTerminalButton->setEnabled(true);
        openInIDEButton->setEnabled(true);
    }
    else
    {
        projectPathLabel->setText("No directory set");

        openInFolderButton->setEnabled(false);
        openInTerminalButton->setEnabled(false);
        openInIDEButton->setEnabled(false);
    }
}

void ProjectDetailsWidget::refreshProcesses()
{
    // Clear existing processes
    QLayoutItem* child;
    while ((child = processListLayout->takeAt(0)) != nullptr)
    {
        if (auto* w = child->widget())
            w->deleteLater();
        delete child;
    }

    // Create process list items
    for (Process& process : currentProcesses)
    {
        ProcessListItem* item = new ProcessListItem(process, repositoryProvider.getProcessRepository(), this);

        connect(item, &ProcessListItem::editRequested, this, &ProjectDetailsWidget::onEditProcessClicked);
        connect(item, &ProcessListItem::deleteRequested, this, &ProjectDetailsWidget::onDeleteProcessClicked);

        processListLayout->addWidget(item);
    }

    processListLayout->addStretch();
}

void ProjectDetailsWidget::onAddNoteClicked()
{
    Note newNote;
    newNote.setProjectId(currentProject.getId());

    NoteDialog dialog(newNote, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Note saved = dialog.getNote();
        noteRepository.save(saved);
        loadProjectNotes(currentProject.getId());
    }
}

void ProjectDetailsWidget::onOpenNoteDialog(const Note& note)
{
    NoteDialog dialog(note, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Note updated = dialog.getNote();
        noteRepository.save(updated);
        loadProjectNotes(currentProject.getId());
    }
}

void ProjectDetailsWidget::onEditProjectClicked()
{
    ProjectDialog dialog(repositoryProvider, this, currentProject);
    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    Project updatedProject = dialog.getProject();
    auto savedProject = projectRepository.save(updatedProject);

    if (!savedProject)
    {
        QMessageBox::critical(this, "Error", "Failed to save project changes.");
        return;
    }

    QList<int> selectedAppIds = dialog.getSelectedAppIds();
    repositoryProvider.getAppRepository().setLinkedApps(savedProject->getId(), selectedAppIds);

    LOG_INFO("Updated project: " + savedProject->getName());

    refreshProject();
}

void ProjectDetailsWidget::onOpenInFolderClicked()
{
    ProjectLauncher::openFolder(currentProject.getDirectoryPath());
}

void ProjectDetailsWidget::onOpenInTerminalClicked()
{
    ProjectLauncher::openTerminal(currentProject.getDirectoryPath());
}

void ProjectDetailsWidget::onOpenInIDEClicked()
{
    if (currentProject.getDirectoryPath().isEmpty())
    {
        QMessageBox::information(this, "No Project Path", "Please set a project directory path first.");
        return;
    }

    QString path = currentProject.getDirectoryPath();
    if (!QDir(path).exists())
    {
        QMessageBox::warning(this, "Invalid Path", QString("The directory '%1' does not exist.").arg(path));
        return;
    }

    QList<Editor> availableEditors = editorRepository.findAll();

    if (availableEditors.isEmpty())
    {
        QMessageBox::information(this, "No Editors", "No editors configured. Add some in Settings.");
        return;
    }

    ChooseEditorDialog dialog(availableEditors, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        ProjectLauncher::openInEditor(currentProject.getDirectoryPath(), dialog.getSelectedEditor());
    }
}

void ProjectDetailsWidget::onOpenAllAppsClicked()
{
    QList<App> apps = repositoryProvider.getAppRepository().findByProjectId(currentProject.getId());

    if (apps.isEmpty())
    {
        QMessageBox::information(this, "No Linked Apps",
                                 "This project has no linked apps.\n\nTo link apps, edit the project and select the "
                                 "desired apps from the list.");
    }

    for (const App& app : apps)
    {
        if (!app.isEnabled())
            continue;

        QString path = app.getPath();
        QString args = app.getArguments();

        if (!QFile::exists(path))
        {
            LOG_WARNING("App not found: " + path);
            continue;
        }

        QProcess* process = new QProcess(this);
        process->setProgram(path);
        if (!args.trimmed().isEmpty())
        {
            process->setArguments(QProcess::splitCommand(args));
        }
        process->startDetached(); // Run independently
    }

    LOG_INFO("Launched all enabled apps for project: " + currentProject.getName());
}

void ProjectDetailsWidget::onAddProcessClicked()
{
    Process newProcess;
    newProcess.setProjectId(currentProject.getId());
    ProcessDialog dialog(newProcess, repositoryProvider, currentProject, this);

    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    Process processToSave = dialog.getProcess();
    processToSave.setProjectId(currentProject.getId());

    std::optional<Process> savedProcess = processRepository.save(processToSave);
    if (!savedProcess.has_value())
    {
        return;
    }

    LOG_INFO("Created process: " + savedProcess->getName());
    loadProjectProcesses(currentProject.getId());
}

void ProjectDetailsWidget::onEditProcessClicked(const Process& process)
{
    ProcessDialog dialog(process, repositoryProvider, currentProject, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Process updatedProcess = dialog.getProcess();
        updatedProcess.setProjectId(currentProject.getId());

        std::optional<Process> savedProcess = processRepository.save(updatedProcess);
        if (savedProcess.has_value())
        {
            LOG_INFO("Updated process: " + savedProcess->getName());
            loadProjectProcesses(currentProject.getId());
        }
    }
}

void ProjectDetailsWidget::onDeleteProcessClicked(const Process& process)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Delete Process", QString("Are you sure you want to delete the process '%1'?").arg(process.getName()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (processRepository.deleteById(process.getId()))
        {
            LOG_INFO("Deleted process: " + process.getName());
            loadProjectProcesses(currentProject.getId());
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to delete process.");
        }
    }
}

void ProjectDetailsWidget::onToggleNotesClicked(bool checked)
{
    toggleNotesBtn->setIcon(checked ? arrowDown : arrowRight);
    notesScrollArea->setVisible(checked);
    settings.setValue("NotesVisible", checked);
}

