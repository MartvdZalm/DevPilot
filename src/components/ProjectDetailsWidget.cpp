#include "ProjectDetailsWidget.h"

#include "../core/Logger.h"
#include "../styles/ButtonStyle.h"
#include "../styles/FontStyle.h"
#include "dialogs/ChooseEditorDialog.h"
#include "FlowLayout.h"
#include "ModuleListItem.h"
#include "NoteCard.h"
#include "dialogs/ModuleEditDialog.h"
#include "dialogs/NoteDialog.h"
#include "dialogs/ProjectDialog.h"
#include "../core/ProjectLauncher.h"
#include <QDesktopServices>
#include <QDir>
#include <QListWidget>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>

ProjectDetailsWidget::ProjectDetailsWidget(RepositoryProvider& repoProvider, QWidget* parent)
    : QWidget(parent), repositoryProvider(repoProvider), projectRepository(repoProvider.getProjectRepository()),
      moduleRepository(repoProvider.getModuleRepository()), noteRepository(repoProvider.getNoteRepository()),
      editorRepository(repoProvider.getEditorRepository())
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

    // Modules section header with toggle button
    QHBoxLayout* moduleControlsLayout = new QHBoxLayout();
    moduleControlsLayout->setContentsMargins(0, 10, 0, 10);

    QLabel* modulesTitle = new QLabel("Modules");
    modulesTitle->setStyleSheet(FontStyle::h2());
    moduleControlsLayout->addWidget(modulesTitle);

    projectDetailsLayout->addLayout(moduleControlsLayout);

    // Scrollable area for modules
    modulesScrollArea = new QScrollArea();
    modulesScrollArea->setWidgetResizable(true);
    modulesScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    modulesScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* modulesContainer = new QWidget();
    modulesContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    moduleListLayout = new QVBoxLayout(modulesContainer);
    moduleListLayout->setContentsMargins(0, 0, 0, 0);
    moduleListLayout->setSpacing(15);
    moduleListLayout->setAlignment(Qt::AlignTop);

    modulesScrollArea->setWidget(modulesContainer);

    projectDetailsLayout->addWidget(modulesScrollArea, 1);

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
    connect(addModuleButton, &QPushButton::clicked, this, &ProjectDetailsWidget::onAddModuleClicked);
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

    addModuleButton = new QPushButton("+");
    addModuleButton->setFixedSize(25, 25);
    addModuleButton->setStyleSheet(ButtonStyle::primary());

    QVBoxLayout* projectInfoLayout = new QVBoxLayout();
    projectInfoLayout->addLayout(titleLayout);
    projectInfoLayout->addWidget(projectPathLabel);
    projectInfoLayout->addWidget(addModuleButton);

    openInFolderButton = new QPushButton("Open");
    openInFolderButton->setStyleSheet(ButtonStyle::primary());

    openInTerminalButton = new QPushButton("Open in Terminal");
    openInTerminalButton->setStyleSheet(ButtonStyle::primary());

    openInIDEButton = new QPushButton("Open in IDE");
    openInIDEButton->setStyleSheet(ButtonStyle::primary());

    projectHeaderLayout->addLayout(projectInfoLayout);
    projectHeaderLayout->addStretch();
    projectHeaderLayout->addWidget(openInFolderButton);
    projectHeaderLayout->addWidget(openInTerminalButton);
    projectHeaderLayout->addWidget(openInIDEButton);

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

    loadProjectModules(projectId);
    loadProjectNotes(projectId);
}

void ProjectDetailsWidget::loadProjectModules(int projectId)
{
    currentModules = moduleRepository.findByProjectId(projectId);
    refreshModules();

    LOG_INFO("Loaded " + QString::number(currentModules.size()) +
             " modules for project ID: " + QString::number(projectId));
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

void ProjectDetailsWidget::refreshModules()
{
    // Clear existing modules
    QLayoutItem* child;
    while ((child = moduleListLayout->takeAt(0)) != nullptr)
    {
        if (auto* w = child->widget())
            w->deleteLater();
        delete child;
    }

    // Create module list items
    for (const Module& module : currentModules)
    {
        ModuleListItem* item = new ModuleListItem(module, this);

        connect(item, &ModuleListItem::editRequested, this, &ProjectDetailsWidget::onEditModuleClicked);
        connect(item, &ModuleListItem::deleteRequested, this, &ProjectDetailsWidget::onDeleteModuleClicked);

        moduleListLayout->addWidget(item);
    }

    moduleListLayout->addStretch();
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
    ProjectDialog dialog(this, currentProject);
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

    LOG_INFO("Updated project: " + savedProject->getName());
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

void ProjectDetailsWidget::onAddModuleClicked()
{
    Module newModule;
    newModule.setProjectId(currentProject.getId());
    ModuleEditDialog dialog(newModule, repositoryProvider, currentProject, this);

    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    Module moduleToSave = dialog.getModule();
    moduleToSave.setProjectId(currentProject.getId());

    std::optional<Module> savedModule = moduleRepository.save(moduleToSave);
    if (!savedModule.has_value())
    {
        return;
    }

    LOG_INFO("Created module: " + savedModule->getName());
    loadProjectModules(currentProject.getId());
}

void ProjectDetailsWidget::onEditModuleClicked(const Module& module)
{
    ModuleEditDialog dialog(module, repositoryProvider, currentProject, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Module updatedModule = dialog.getModule();
        updatedModule.setProjectId(currentProject.getId());

        std::optional<Module> savedModule = moduleRepository.save(updatedModule);
        if (savedModule.has_value())
        {
            LOG_INFO("Updated module: " + savedModule->getName());
            loadProjectModules(currentProject.getId());
        }
    }
}

void ProjectDetailsWidget::onDeleteModuleClicked(const Module& module)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Delete Module", QString("Are you sure you want to delete the module '%1'?").arg(module.getName()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (moduleRepository.deleteById(module.getId()))
        {
            LOG_INFO("Deleted module: " + module.getName());
            loadProjectModules(currentProject.getId());
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to delete module.");
        }
    }
}

void ProjectDetailsWidget::onToggleNotesClicked(bool checked)
{
    toggleNotesBtn->setIcon(checked ? arrowDown : arrowRight);
    notesScrollArea->setVisible(checked);
}
