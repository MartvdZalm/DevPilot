#include "HomeWindow.h"
#include "../components/FlowLayout.h"
#include "../components/ModuleListItem.h"
#include "../components/NoteCard.h"
#include "../components/dialogs/ModuleEditDialog.h"
#include "../components/dialogs/NoteDialog.h"
#include "../components/dialogs/ProjectDialog.h"
#include "../core/Logger.h"
#include "../styles/ButtonStyle.h"
#include "../styles/ListStyle.h"
#include "../database/Container.h"
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QListWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QSplitter>
#include <QTextEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <windows.h>

HomeWindow::HomeWindow(QWidget* parent) : BaseWindow(parent)
{
    projectRepository = Container::instance().getProjectRepository();
    moduleRepository = Container::instance().getModuleRepository();
    noteRepository = Container::instance().getNoteRepository();

    setupUI();
    setupConnections();
    loadProjects();
}

void HomeWindow::setupUI()
{
    auto* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* sidebarWidget = new QWidget();
    sidebarWidget->setFixedWidth(250);
    sidebarWidget->setStyleSheet("border-right: 1px solid #3a3f47;");

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(15, 15, 15, 15);
    sidebarLayout->setSpacing(10);

    QHBoxLayout* projectsHeaderLayout = new QHBoxLayout();
    QLabel* projectsLabel = new QLabel("Projects");
    projectsLabel->setStyleSheet("font-size: 18px; font-weight: bold; border: none;");

    addProjectButton = new QPushButton(QIcon(":/Images/Add"), "", this);
    addProjectButton->setStyleSheet(ButtonStyle::icon());
    addProjectButton->setIconSize(QSize(25, 25));

    projectsHeaderLayout->addWidget(projectsLabel);
    projectsHeaderLayout->addStretch();
    projectsHeaderLayout->addWidget(addProjectButton);
    sidebarLayout->addLayout(projectsHeaderLayout);

    projectList = new QListWidget();
    projectList->setStyleSheet(ListStyle::primary());

    sidebarLayout->addWidget(projectList);
    sidebarLayout->addStretch();

    mainLayout->addWidget(sidebarWidget);

    emptyStateWidget = new QWidget();
    QVBoxLayout* emptyLayout = new QVBoxLayout(emptyStateWidget);
    emptyLayout->setAlignment(Qt::AlignCenter);

    QLabel* emptyTitle = new QLabel("No project selected");
    emptyTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #aaaaaa;");
    emptyTitle->setAlignment(Qt::AlignCenter);
    QLabel* emptySubtitle = new QLabel("Create a new project to get started.\n"
                                       "You can then add modules, run scripts, and open it in your IDE.");
    emptySubtitle->setStyleSheet("font-size: 14px; color: #888888;");
    emptySubtitle->setAlignment(Qt::AlignCenter);

    QPushButton* createProjectBtn = new QPushButton("Create Project");
    createProjectBtn->setStyleSheet(ButtonStyle::primary());
    connect(createProjectBtn, &QPushButton::clicked, this, &HomeWindow::onAddProjectClicked);

    emptyLayout->addWidget(emptyTitle);
    emptyLayout->addWidget(emptySubtitle);
    emptyLayout->addSpacing(15);
    emptyLayout->addWidget(createProjectBtn, 0, Qt::AlignCenter);

    QWidget* mainContentWidget = new QWidget();
    QVBoxLayout* mainContentLayout = new QVBoxLayout(mainContentWidget);
    mainContentLayout->setContentsMargins(20, 20, 20, 20);
    mainContentLayout->setSpacing(15);

    projectDetailsWidget = new QWidget();
    QVBoxLayout* projectDetailsLayout = new QVBoxLayout(projectDetailsWidget);
    projectDetailsLayout->setContentsMargins(0, 0, 0, 0);
    projectDetailsLayout->setSpacing(15);

    QHBoxLayout* projectHeaderLayout = new QHBoxLayout();

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(8);

    projectNameLabel = new QLabel();
    projectNameLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #ffffff;");

    QPushButton* editProjectButton = new QPushButton(QIcon(":/Images/Edit"), "");
    editProjectButton->setStyleSheet(ButtonStyle::icon());
    editProjectButton->setIconSize(QSize(20, 20));
    connect(editProjectButton, &QPushButton::clicked, this, &HomeWindow::onEditProjectClicked);

    titleLayout->addWidget(projectNameLabel);
    titleLayout->addWidget(editProjectButton);
    titleLayout->addStretch();

    projectPathLabel = new QLabel();
    projectPathLabel->setStyleSheet("font-size: 12px; color: #a0a0a0; margin-top: 5px;");

    addModuleButton = new QPushButton("+");
    addModuleButton->setFixedSize(25, 25);
    addModuleButton->setStyleSheet(ButtonStyle::primary());

    QVBoxLayout* projectInfoLayout = new QVBoxLayout();
    projectInfoLayout->addLayout(titleLayout);
    projectInfoLayout->addWidget(projectPathLabel);
    projectInfoLayout->addWidget(addModuleButton);

    QPushButton* openFolderButton = new QPushButton("Open");
    openFolderButton->setStyleSheet(ButtonStyle::primary());
    connect(openFolderButton, &QPushButton::clicked, this, &HomeWindow::onOpenInFolderClicked);

    QPushButton* openTerminalButton = new QPushButton("Open in Terminal");
    openTerminalButton->setStyleSheet(ButtonStyle::primary());
    connect(openTerminalButton, &QPushButton::clicked, this, &HomeWindow::onOpenInTerminalClicked);

    QPushButton* openIDEButton = new QPushButton("Open in IDE");
    openIDEButton->setStyleSheet(ButtonStyle::primary());
    connect(openIDEButton, &QPushButton::clicked, this, &HomeWindow::onOpenInIDEClicked);

    projectHeaderLayout->addLayout(projectInfoLayout);
    projectHeaderLayout->addStretch();
    projectHeaderLayout->addWidget(openFolderButton);
    projectHeaderLayout->addWidget(openTerminalButton);
    projectHeaderLayout->addWidget(openIDEButton);

    projectDetailsLayout->addLayout(projectHeaderLayout);

    QWidget* moduleListWidget = new QWidget();
    moduleListLayout = new QVBoxLayout(moduleListWidget);
    moduleListLayout->setContentsMargins(0, 0, 0, 0);
    moduleListLayout->setSpacing(15);

    projectDetailsLayout->addWidget(moduleListWidget);
    projectDetailsLayout->addStretch();

    QToolButton* toggleNotesBtn = new QToolButton();
    toggleNotesBtn->setStyleSheet(ButtonStyle::icon());
    toggleNotesBtn->setIconSize(QSize(24, 24));
    toggleNotesBtn->setText("Notes");
    toggleNotesBtn->setCheckable(true);
    toggleNotesBtn->setChecked(true);
    toggleNotesBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QIcon arrowDown(":/Images/ArrowDropDown");
    QIcon arrowRight(":/Images/ArrowDropUp");
    toggleNotesBtn->setIcon(arrowDown);

    notesScrollArea = new QScrollArea();
    notesScrollArea->setMaximumHeight(250);
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

    connect(addNoteButton, &QToolButton::clicked, this, &HomeWindow::onAddNoteClicked);

    notesListLayout->addWidget(addNoteButton);

    connect(toggleNotesBtn, &QToolButton::toggled, this,
            [=](bool checked)
            {
                toggleNotesBtn->setIcon(checked ? arrowDown : arrowRight);
                notesScrollArea->setVisible(checked);
            });

    projectDetailsLayout->addWidget(toggleNotesBtn);
    projectDetailsLayout->addWidget(notesScrollArea);

    projectDetailsLayout->addWidget(notesScrollArea);

    mainContentLayout->addWidget(emptyStateWidget);
    mainContentLayout->addWidget(projectDetailsWidget);
    mainLayout->addWidget(mainContentWidget);

    projectDetailsWidget->hide();
    emptyStateWidget->show();
}

void HomeWindow::setupConnections()
{
    connect(addProjectButton, &QPushButton::clicked, this, &HomeWindow::onAddProjectClicked);
    connect(projectList, &QListWidget::itemClicked, this, &HomeWindow::onProjectSelected);
    connect(addModuleButton, &QPushButton::clicked, this,
            [this]()
            {
                ModuleEditDialog dialog(this);
                if (dialog.exec() != QDialog::Accepted)
                {
                    return;
                }

                Module newModule = dialog.getModule();
                newModule.setProjectId(currentProject.getId());

                std::optional<Module> savedModule = moduleRepository->save(newModule);
                if (!savedModule.has_value())
                {
                    return;
                }

                LOG_INFO("Created module: " + savedModule->getName());

                refreshModuleDisplay();
            });
}

void HomeWindow::onAddProjectClicked()
{
    ProjectDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    auto savedProject = projectRepository->save(dialog.getProject());
    if (!savedProject)
    {
        return;
    }

    LOG_INFO("Created project: " + savedProject->getName());

    refreshProjectList();

    for (int i = 0; i < projectList->count(); ++i)
    {
        auto* item = projectList->item(i);
        if (item->data(Qt::UserRole).toInt() == savedProject->getId())
        {
            projectList->setCurrentItem(item);
            onProjectSelected(item);
            break;
        }
    }
}

void HomeWindow::onEditProjectClicked()
{
    if (!currentProject.getId())
    {
        QMessageBox::warning(this, "No project selected", "Please select a project to edit.");
        return;
    }

    ProjectDialog dialog(this, currentProject);
    if (dialog.exec() != QDialog::Accepted)
        return;

    Project updatedProject = dialog.getProject();
    auto savedProject = projectRepository->save(updatedProject);
    if (!savedProject)
    {
        QMessageBox::critical(this, "Error", "Failed to save project changes.");
        return;
    }

    LOG_INFO("Updated project: " + savedProject->getName());

    refreshProjectList();

    for (int i = 0; i < projectList->count(); ++i)
    {
        auto* item = projectList->item(i);
        if (item->data(Qt::UserRole).toInt() == savedProject->getId())
        {
            projectList->setCurrentItem(item);
            onProjectSelected(item);
            break;
        }
    }
}

void HomeWindow::onOpenInFolderClicked()
{
    if (currentProject.getDirectoryPath().isEmpty())
    {
        QMessageBox::information(this, "No Project Path",
                                 "Please set a project directory path first. "
                                 "You can do this by editing the project.");
        return;
    }

    QString path = currentProject.getDirectoryPath();
    if (!QDir(path).exists())
    {
        QMessageBox::warning(this, "Invalid Path",
                             QString("The directory '%1' does not exist.").arg(path));
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void HomeWindow::onOpenInTerminalClicked()
{
    if (currentProject.getDirectoryPath().isEmpty())
    {
        QMessageBox::information(this, "No Project Path",
                                 "Please set a project directory path first. "
                                 "You can do this by editing the project.");
        return;
    }
#if defined(Q_OS_WIN)
    QString path = currentProject.getDirectoryPath();
    if (path.isEmpty())
    {
        return;
    }

    if (path.contains("wsl", Qt::CaseInsensitive))
    {
        QRegularExpression regex(R"(\\\\wsl\.localhost\\[^\\]+\\home\\[^\\]+)");
        QString cleanedPath = path.replace(regex, "~");
        cleanedPath.replace("\\", "/");

        QStringList arguments;
        arguments << "wsl" << "--cd" << cleanedPath;

        QProcess::startDetached("wt.exe", arguments);
    } else {
        QString windowsPath = QDir::toNativeSeparators(path).trimmed();
        if (windowsPath.endsWith('\\'))
        {
            windowsPath.chop(1);
        }

        QString command = QString("Start-Process cmd -ArgumentList '/K cd /d \"\"%1\"\"'").arg(windowsPath);
        QProcess::startDetached("powershell.exe", QStringList() << "-Command" << command);

    }
#endif
}

void HomeWindow::onOpenInIDEClicked()
{
    qDebug() << "Open IDE";

    if (currentProject.getDirectoryPath().isEmpty())
    {
        QMessageBox::information(this, "No Project Path",
                                 "Please set a project directory path first. "
                                 "You can do this by editing the project.");
        return;
    }

    QString path = currentProject.getDirectoryPath();
    if (!QDir(path).exists())
    {
        QMessageBox::warning(this, "Invalid Path",
                             QString("The directory '%1' does not exist.").arg(path));
        return;
    }

    QProcess::startDetached("code", QStringList() << path);
}

void HomeWindow::onProjectSelected(QListWidgetItem* item)
{
    if (!item)
    {
        return;
    }

    if (openedProjectItem == item)
    {
        item->setIcon(QIcon(":/Images/Folder"));

        currentProject = Project();

        projectDetailsWidget->hide();
        emptyStateWidget->show();

        openedProjectItem = nullptr;
        return;
    }

    for (int i = 0; i < projectList->count(); ++i)
    {
        projectList->item(i)->setIcon(QIcon(":/Images/Folder"));
    }

    item->setIcon(QIcon(":/Images/FolderOpen"));

    int projectId = item->data(Qt::UserRole).toInt();
    std::optional<Project> project = projectRepository->findById(projectId);
    if (!project.has_value())
    {
        return;
    }

    currentProject = project.value();
    openedProjectItem = item;

    projectNameLabel->setText(currentProject.getName());
    if (!currentProject.getDirectoryPath().isEmpty())
    {
        projectPathLabel->setText(QString("Directory  %1").arg(currentProject.getDirectoryPath()));
    }
    else
    {
        projectPathLabel->setText("");
    }

    projectDetailsWidget->show();
    emptyStateWidget->hide();

    loadProjectModules(projectId);
    loadProjectNotes(projectId);
}

void HomeWindow::loadProjects()
{
    if (!projectRepository)
        return;

    const auto projects = projectRepository->findAll();

    projectList->clear();
    for (const auto& project : projects)
    {
        auto* item = new QListWidgetItem(QIcon(":/Images/Folder"), project.getName());
        item->setData(Qt::UserRole, project.getId());
        projectList->addItem(item);
    }

    LOG_INFO("Loaded " + QString::number(projects.size()) + " projects");
}

void HomeWindow::loadProjectModules(int projectId)
{
    if (!moduleRepository)
        return;

    currentModules = moduleRepository->findByProjectId(projectId);
    refreshModuleDisplay();

    LOG_INFO("Loaded " + QString::number(currentModules.size()) +
             " modules for project ID: " + QString::number(projectId));
}

void HomeWindow::loadProjectNotes(int projectId)
{
    if (!noteRepository)
        return;

    QList<Note> currentNotes = noteRepository->findByProjectId(projectId);

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

    for (const Note& note : currentNotes)
    {
        NoteCard* card = new NoteCard(note, notesContainer);

        connect(card, &NoteCard::clicked, this, [this](const Note& n) { openNoteDialog(n); });

        connect(card, &NoteCard::deleteClicked, this,
                [this](const Note& n)
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Delete Note",
                                                  QString("Are you sure you want to delete '%1'?").arg(n.getTitle()),
                                                  QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        noteRepository->deleteById(n.getId());
                        loadProjectNotes(currentProject.getId());
                    }
                });

        notesListLayout->addWidget(card);
    }
}

void HomeWindow::onAddNoteClicked()
{
    Note newNote;
    newNote.setProjectId(currentProject.getId());

    NoteDialog dialog(newNote, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Note saved = dialog.getNote();
        noteRepository->save(saved);
        loadProjectNotes(currentProject.getId());
    }
}

void HomeWindow::openNoteDialog(const Note& note)
{
    NoteDialog dialog(note, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Note updated = dialog.getNote();
        noteRepository->save(updated);
        loadProjectNotes(currentProject.getId());
    }
}

void HomeWindow::refreshProjectList()
{
    loadProjects();
}

void HomeWindow::refreshModuleDisplay()
{
    QLayoutItem* child;
    while ((child = moduleListLayout->takeAt(0)) != nullptr)
    {
        if (auto* w = child->widget())
            w->deleteLater();
        delete child;
    }

    for (const Module& module : currentModules)
    {
        const QString icon = statusIconForModule(module);
        ModuleListItem* item = new ModuleListItem(module, icon, this);

        if (!module.getLogs().isEmpty())
            item->setLogs(module.getLogs());

        if (QPushButton* startBtn = item->getStartButton())
        {
            connect(startBtn, &QPushButton::clicked, this, [this, item]() { item->startCommand(); });
        }
        if (QPushButton* stopBtn = item->getStopButton())
        {
            connect(stopBtn, &QPushButton::clicked, this, [this, item]() { item->stopCommand(); });
        }

        moduleListLayout->addWidget(item);
    }

    moduleListLayout->addStretch();
}

QString HomeWindow::statusIconForModule(const Module& m) const
{
    try
    {
        switch (m.getStatus())
        {
        case Module::Status::Running:
            return QStringLiteral("🟢");
        case Module::Status::Stopped:
            return QStringLiteral("🔴");
        default:
            return QStringLiteral("⚪️");
        }
    }
    catch (...)
    {
        return QStringLiteral("⚪️");
    }
}

void HomeWindow::onModuleStartClicked(int moduleId)
{
    LOG_INFO(QString("Start clicked for module ID: %1").arg(moduleId));
}

void HomeWindow::onModuleStopClicked(int moduleId)
{
    LOG_INFO(QString("Stop clicked for module ID: %1").arg(moduleId));
}
