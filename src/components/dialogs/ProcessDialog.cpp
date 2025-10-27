#include "ProcessDialog.h"
#include "../../styles/ButtonStyle.h"
#include "../../styles/InputStyle.h"
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

ProcessDialog::ProcessDialog(const Process& process, RepositoryProvider& repositoryProvider, Project project,
                             QWidget* parent)
    : QDialog(parent), process(process), processRepository(repositoryProvider.getProcessRepository()),
      processTemplateRepository(repositoryProvider.getProcessTemplateRepository()), project(project)
{
    setupUI();
    setupConnections();
    setProcess(process);
}

void ProcessDialog::setupUI()
{
    bool isEditMode = process.getId() != 0;
    setWindowTitle(isEditMode ? "Edit Process" : "Add Process");
    setModal(true);
    setMinimumWidth(500);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(18);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    formLayout->setSpacing(12);

    templateComboBox = new QComboBox();
    templateComboBox->setStyleSheet(InputStyle::primary());
    formLayout->addRow("Template:", templateComboBox);

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("e.g. Backend Server");
    nameEdit->setStyleSheet(InputStyle::primary());
    formLayout->addRow("Name:", nameEdit);

    portSpinBox = new QSpinBox();
    portSpinBox->setRange(1, 65535);
    portSpinBox->setStyleSheet(InputStyle::primary());
    formLayout->addRow("Port:", portSpinBox);

    commandEdit = new QLineEdit();
    commandEdit->setPlaceholderText("e.g. npm run dev");
    commandEdit->setStyleSheet(InputStyle::primary());
    formLayout->addRow("Command:", commandEdit);

    workingDirEdit = new QLineEdit();
    workingDirEdit->setPlaceholderText("Leave empty for project root");
    workingDirEdit->setStyleSheet(InputStyle::primary());

    browseButton = new QPushButton(QIcon(":/Images/Folder"), "");
    browseButton->setStyleSheet(ButtonStyle::primary());

    QHBoxLayout* workingDirLayout = new QHBoxLayout();
    workingDirLayout->addWidget(workingDirEdit);
    workingDirLayout->addWidget(browseButton);
    formLayout->addRow("Directory:", workingDirLayout);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel");
    cancelButton->setStyleSheet(ButtonStyle::primary());
    okButton = new QPushButton("Ok");
    okButton->setStyleSheet(ButtonStyle::primary());

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    loadTemplatesFromRepository();
}

void ProcessDialog::setupConnections()
{
    connect(okButton, &QPushButton::clicked, this, &ProcessDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ProcessDialog::onCancelClicked);
    connect(templateComboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this,
            &ProcessDialog::onTemplateSelected);
    connect(browseButton, &QPushButton::clicked, this, &ProcessDialog::onBrowseClicked);
}

void ProcessDialog::onBrowseClicked()
{
    QString currentDir = workingDirEdit->text().isEmpty() ? project.getDirectoryPath() : workingDirEdit->text();

    QString selectedDir = QFileDialog::getExistingDirectory(
        this, "Select Working Directory", currentDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!selectedDir.isEmpty())
    {
        workingDirEdit->setText(selectedDir);
    }
}

void ProcessDialog::loadTemplatesFromRepository()
{
    templateComboBox->clear();
    processTemplateMap.clear();

    templateComboBox->addItem("Select a template...");

    auto processTemplates = processTemplateRepository.findAll();

    for (const auto& processTemplate : processTemplates)
    {
        if (processTemplate.isEnabled())
        {
            templateComboBox->addItem(processTemplate.getName());
            processTemplateMap[processTemplate.getName()] = processTemplate;
        }
    }

    templateComboBox->setCurrentIndex(0);
}

Process ProcessDialog::getProcess()
{
    return process;
}

void ProcessDialog::setProcess(const Process& process)
{
    this->process = process;
    nameEdit->setText(process.getName());
    portSpinBox->setValue(process.getPort());
    commandEdit->setText(process.getCommand());
    workingDirEdit->setText(process.getWorkingDirectory());
}

void ProcessDialog::onOkClicked()
{
    QString name = nameEdit->text().trimmed();
    QString command = commandEdit->text().trimmed();
    QString dir = workingDirEdit->text().trimmed();

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Process name is required!");
        return;
    }

    if (command.isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Command is required!");
        return;
    }

    process.setName(name);
    process.setCommand(command);
    process.setWorkingDirectory(dir);
    process.setPort(portSpinBox->value());

    accept();
}

void ProcessDialog::onCancelClicked()
{
    reject();
}

void ProcessDialog::onTemplateSelected(const QString& displayName)
{
    if (processTemplateMap.contains(displayName))
    {
        const ProcessTemplate& selectedTemplate = processTemplateMap[displayName];
        applyTemplate(selectedTemplate);
    }
}

void ProcessDialog::applyTemplate(const ProcessTemplate& tpl)
{
    if (nameEdit->text().isEmpty())
    {
        nameEdit->setText(tpl.getName());
    }
    if (commandEdit->text().isEmpty())
    {
        commandEdit->setText(tpl.getCommand());
    }
    portSpinBox->setValue(tpl.getPort());
}
