#include "ProjectDialog.h"
#include "../../styles/ButtonStyle.h"
#include "../../styles/InputStyle.h"
#include "../../styles/GroupBoxStyle.h"
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

ProjectDialog::ProjectDialog(RepositoryProvider& repoProvider, QWidget* parent, const Project& projectToEdit)
    : QDialog(parent), project(projectToEdit), repositoryProvider(repoProvider)
{
    editing = !projectToEdit.getName().isEmpty();
    setupUI();
    setupConnections();
    loadApps(); // load app list before filling checkboxes
    populateFields();
}

void ProjectDialog::setupUI()
{
    setWindowTitle(editing ? "Edit Project" : "Create New Project");
    setModal(true);
    setMinimumSize(400, 300);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    QFormLayout* infoLayout = new QFormLayout();

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Name");
    nameEdit->setStyleSheet(InputStyle::primary());
    infoLayout->addRow("Project Name:", nameEdit);

    QHBoxLayout* pathLayout = new QHBoxLayout();
    pathEdit = new QLineEdit();
    pathEdit->setPlaceholderText("Folder path");
    pathEdit->setStyleSheet(InputStyle::primary());

    browseButton = new QPushButton("Browse...");
    browseButton->setStyleSheet(ButtonStyle::primary());

    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseButton);
    infoLayout->addRow("Project Path:", pathLayout);

    descriptionEdit = new QTextEdit();
    descriptionEdit->setMaximumHeight(80);
    descriptionEdit->setStyleSheet(InputStyle::primary());
    infoLayout->addRow("Description:", descriptionEdit);

    mainLayout->addLayout(infoLayout);

    QGroupBox* appGroup = new QGroupBox("Linked Apps");
    appGroup->setStyleSheet(GroupBoxStyle::primary());
    QVBoxLayout* appLayout = new QVBoxLayout();

    appListWidget = new QListWidget();
    appListWidget->setStyleSheet("background: transparent; border: none;");
    appListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    appLayout->addWidget(appListWidget);

    appGroup->setLayout(appLayout);
    mainLayout->addWidget(appGroup);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel");
    cancelButton->setStyleSheet(ButtonStyle::primary());
    okButton = new QPushButton(editing ? "Save Changes" : "Create Project");
    okButton->setStyleSheet(ButtonStyle::primary());

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    mainLayout->addLayout(buttonLayout);
}

void ProjectDialog::setupConnections()
{
    connect(okButton, &QPushButton::clicked, this, &ProjectDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ProjectDialog::onCancelClicked);
    connect(browseButton, &QPushButton::clicked,
            [this]()
            {
                QString dir = QFileDialog::getExistingDirectory(this, "Select Project Folder");
                if (!dir.isEmpty())
                {
                    QString windowsPath = QDir::toNativeSeparators(dir);
                    pathEdit->setText(windowsPath);
                }
            });
}

void ProjectDialog::loadApps()
{
    allApps = repositoryProvider.getAppRepository().findAll();

    for (const App& app : allApps)
    {
        QListWidgetItem* item = new QListWidgetItem(app.getName(), appListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setData(Qt::UserRole, app.getId());
    }
}

void ProjectDialog::populateFields()
{
    if (!editing)
        return;

    nameEdit->setText(project.getName());
    pathEdit->setText(project.getDirectoryPath());
    descriptionEdit->setText(project.getDescription());

    QList<App> linkedApps = repositoryProvider.getAppRepository().findByProjectId(project.getId());
    QSet<int> linkedAppIds;
    for (const App& app : linkedApps)
    {
        linkedAppIds.insert(app.getId());
    }

    for (int i = 0; i < appListWidget->count(); ++i)
    {
        QListWidgetItem* item = appListWidget->item(i);
        int appId = item->data(Qt::UserRole).toInt();

        if (linkedAppIds.contains(appId))
        {
            item->setCheckState(Qt::Checked);
        }
    }
}

Project ProjectDialog::getProject() const
{
    return project;
}

QList<int> ProjectDialog::getSelectedAppIds() const
{
    QList<int> selectedAppIds;
    for (int i = 0; i < appListWidget->count(); ++i)
    {
        QListWidgetItem* item = appListWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            selectedAppIds.append(item->data(Qt::UserRole).toInt());
        }
    }
    return selectedAppIds;
}

void ProjectDialog::onOkClicked()
{
    if (nameEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Project name is required!");
        return;
    }

    QString originalPath = pathEdit->text().trimmed();
    if (originalPath.isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Project path is required!");
        return;
    }

    project.setName(nameEdit->text().trimmed());
    project.setDirectoryPath(originalPath);
    project.setDescription(descriptionEdit->toPlainText().trimmed());

    accept();
}

void ProjectDialog::onCancelClicked()
{
    reject();
}
