#include "ProjectDialog.h"
#include "../../styles/ButtonStyle.h"
#include "../../styles/InputStyle.h"
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

ProjectDialog::ProjectDialog(QWidget* parent, const Project& projectToEdit) : QDialog(parent), project(projectToEdit)
{
    editing = !projectToEdit.getName().isEmpty();
    setupUI();
    setupConnections();
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
    nameEdit->setPlaceholderText("e.g., My Awesome Project, E-commerce Platform");
    nameEdit->setStyleSheet(InputStyle::primary());
    infoLayout->addRow("Project Name:", nameEdit);

    QHBoxLayout* pathLayout = new QHBoxLayout();

    pathEdit = new QLineEdit();
    pathEdit->setPlaceholderText("Optional: Project folder path");
    pathEdit->setStyleSheet(InputStyle::primary());

    browseButton = new QPushButton("Browse...");
    browseButton->setStyleSheet(ButtonStyle::primary());
    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseButton);
    infoLayout->addRow("Project Path:", pathLayout);

    descriptionEdit = new QTextEdit();
    descriptionEdit->setMaximumHeight(80);
    descriptionEdit->setPlaceholderText("Describe your project...");
    descriptionEdit->setStyleSheet(InputStyle::primary());
    infoLayout->addRow("Description:", descriptionEdit);

    mainLayout->addLayout(infoLayout);

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

void ProjectDialog::populateFields()
{
    if (!editing)
        return;

    nameEdit->setText(project.getName());
    pathEdit->setText(project.getDirectoryPath());
    descriptionEdit->setText(project.getDescription());
}

Project ProjectDialog::getProject() const
{
    return project;
}

void ProjectDialog::onOkClicked()
{
    if (nameEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Project name is required!");
        return;
    }

    QString originalPath = pathEdit->text().trimmed();

    project.setName(nameEdit->text().trimmed());
    project.setDirectoryPath(originalPath);
    project.setDescription(descriptionEdit->toPlainText().trimmed());

    accept();
}

void ProjectDialog::onCancelClicked()
{
    reject();
}
