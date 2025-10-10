#include "ModuleEditDialog.h"
#include "../../styles/ButtonStyle.h"
#include "../../styles/GroupBoxStyle.h"
#include "../../styles/InputStyle.h"
#include "../../core/Logger.h"
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

ModuleEditDialog::ModuleEditDialog(const Module& module, RepositoryProvider& repositoryProvider, Project project,
                                   QWidget* parent)
    : QDialog(parent), module(module), moduleRepository(repositoryProvider.getModuleRepository()),
      moduleTemplateRepository(repositoryProvider.getModuleTemplateRepository()), project(project)
{
    setupUI();
    setupConnections();
    setModule(module);
}

void ModuleEditDialog::setupUI()
{
    setWindowTitle("Edit Module");
    setModal(true);
    setMinimumSize(500, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    QGroupBox* basicGroup = new QGroupBox("Basic Information");
    basicGroup->setStyleSheet(GroupBoxStyle::primary());
    QFormLayout* basicLayout = new QFormLayout(basicGroup);

    templateComboBox = new QComboBox();
    templateComboBox->setStyleSheet(InputStyle::primary());

    loadTemplatesFromRepository();

    basicLayout->addRow("Template:", templateComboBox);

    nameEdit = new QLineEdit();
    nameEdit->setStyleSheet(InputStyle::primary());
    basicLayout->addRow("Name:", nameEdit);

    serviceTypeComboBox = new QComboBox();
    serviceTypeComboBox->setStyleSheet(InputStyle::primary());
    setupServiceTypeComboBox();
    basicLayout->addRow("Service Type:", serviceTypeComboBox);

    portSpinBox = new QSpinBox();
    portSpinBox->setRange(1, 65535);
    portSpinBox->setValue(3000);
    portSpinBox->setStyleSheet(InputStyle::primary());
    basicLayout->addRow("Port:", portSpinBox);

    descriptionEdit = new QTextEdit();
    descriptionEdit->setMaximumHeight(60);
    descriptionEdit->setStyleSheet(InputStyle::primary());
    basicLayout->addRow("Description:", descriptionEdit);

    mainLayout->addWidget(basicGroup);

    QGroupBox* execGroup = new QGroupBox("Execution Settings");
    execGroup->setStyleSheet(GroupBoxStyle::primary());
    QFormLayout* execLayout = new QFormLayout(execGroup);

    commandEdit = new QLineEdit();
    commandEdit->setStyleSheet(InputStyle::primary());
    execLayout->addRow("Command:", commandEdit);

    QHBoxLayout* workingDirLayout = new QHBoxLayout();
    workingDirEdit = new QLineEdit();
    workingDirEdit->setPlaceholderText("Working directory (leave empty for project root)");
    workingDirEdit->setStyleSheet(InputStyle::primary());

    browseButton = new QPushButton("Browse...");
    browseButton->setStyleSheet(ButtonStyle::primary());
    browseButton->setFixedWidth(80);

    workingDirLayout->addWidget(workingDirEdit);
    workingDirLayout->addWidget(browseButton);

    execLayout->addRow("Working Directory:", workingDirLayout);

    parametersEdit = new QTextEdit();
    parametersEdit->setMaximumHeight(60);
    parametersEdit->setPlaceholderText("Additional parameters (one per line)");
    parametersEdit->setStyleSheet(InputStyle::primary());
    execLayout->addRow("Parameters:", parametersEdit);

    environmentEdit = new QTextEdit();
    environmentEdit->setMaximumHeight(60);
    environmentEdit->setPlaceholderText("Environment variables (KEY=VALUE, one per line)");
    environmentEdit->setStyleSheet(InputStyle::primary());
    execLayout->addRow("Environment:", environmentEdit);

    autoStartCheckBox = new QCheckBox("Start automatically with project");
    execLayout->addRow("", autoStartCheckBox);

    mainLayout->addWidget(execGroup);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    okButton = new QPushButton("OK");
    okButton->setStyleSheet(ButtonStyle::primary());
    cancelButton = new QPushButton("Cancel");
    cancelButton->setStyleSheet(ButtonStyle::primary());

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);
}

void ModuleEditDialog::setupConnections()
{
    connect(okButton, &QPushButton::clicked, this, &ModuleEditDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ModuleEditDialog::onCancelClicked);
    connect(templateComboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this,
            &ModuleEditDialog::onTemplateSelected);
    connect(browseButton, &QPushButton::clicked, this, &ModuleEditDialog::onBrowseClicked);
}

void ModuleEditDialog::setupServiceTypeComboBox()
{
    serviceTypeComboBox->clear();
    serviceTypeComboBox->addItem("Custom", static_cast<int>(Module::ServiceType::CUSTOM));
    serviceTypeComboBox->addItem("Backend", static_cast<int>(Module::ServiceType::BACKEND));
    serviceTypeComboBox->addItem("Frontend", static_cast<int>(Module::ServiceType::FRONTEND));
    serviceTypeComboBox->addItem("Database", static_cast<int>(Module::ServiceType::DATABASE));
    serviceTypeComboBox->addItem("Cache", static_cast<int>(Module::ServiceType::CACHE));
    serviceTypeComboBox->addItem("Infrastructure", static_cast<int>(Module::ServiceType::INFRASTRUCTURE));
    serviceTypeComboBox->setCurrentIndex(0);
}

void ModuleEditDialog::onBrowseClicked()
{
    QString currentDir = workingDirEdit->text().isEmpty() ? project.getDirectoryPath() : workingDirEdit->text();

    QString selectedDir = QFileDialog::getExistingDirectory(
        this, "Select Working Directory", currentDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!selectedDir.isEmpty())
    {
        workingDirEdit->setText(selectedDir);
    }
}

void ModuleEditDialog::loadTemplatesFromRepository()
{
    templateComboBox->clear();
    moduleTemplateMap.clear();

    templateComboBox->addItem("Select a template...");

    auto moduleTemplates = moduleTemplateRepository.findAll();

    for (const auto& moduleTemplate : moduleTemplates)
    {
        if (moduleTemplate.isEnabled())
        {
            templateComboBox->addItem(moduleTemplate.getName());
            moduleTemplateMap[moduleTemplate.getName()] = moduleTemplate;
        }
    }

    templateComboBox->setCurrentIndex(0);
}

Module ModuleEditDialog::getModule()
{
    return module;
}

void ModuleEditDialog::setModule(const Module& module)
{
    this->module = module;

    nameEdit->setText(module.getName());
    portSpinBox->setValue(module.getPort());
    commandEdit->setText(module.getCommand());
    workingDirEdit->setText(module.getWorkingDirectory());
    descriptionEdit->setPlainText(module.getDescription());
    parametersEdit->setPlainText(module.getParameters());
    environmentEdit->setPlainText(module.getEnvironment());
    serviceTypeComboBox->setCurrentIndex(static_cast<int>(module.getServiceType()));
}

void ModuleEditDialog::onOkClicked()
{
    if (nameEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Module name is required!");
        return;
    }

    if (commandEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Command is required!");
        return;
    }

    module.setName(nameEdit->text().trimmed());
    module.setPort(portSpinBox->value());
    module.setCommand(commandEdit->text().trimmed());
    module.setWorkingDirectory(workingDirEdit->text().trimmed());
    module.setDescription(descriptionEdit->toPlainText().trimmed());
    module.setParameters(parametersEdit->toPlainText().trimmed());
    module.setEnvironment(environmentEdit->toPlainText().trimmed());

    int serviceTypeValue = serviceTypeComboBox->currentData().toInt();
    module.setServiceType(static_cast<Module::ServiceType>(serviceTypeValue));

    accept();
}

void ModuleEditDialog::onCancelClicked()
{
    reject();
}

void ModuleEditDialog::onTemplateSelected(const QString& displayName)
{
    if (moduleTemplateMap.contains(displayName))
    {
        const ModuleTemplate& selectedTemplate = moduleTemplateMap[displayName];
        applyTemplate(selectedTemplate);
    }
}

void ModuleEditDialog::applyTemplate(const ModuleTemplate& moduleTemplate)
{
    auto nameCursor = nameEdit->cursorPosition();
    auto descCursor = descriptionEdit->textCursor().position();
    auto cmdCursor = commandEdit->cursorPosition();

    nameEdit->setText(moduleTemplate.getName());
    nameEdit->setCursorPosition(nameCursor);

    commandEdit->setText(moduleTemplate.getCommand());
    commandEdit->setCursorPosition(cmdCursor);

    portSpinBox->setValue(moduleTemplate.getPort());

    if (descriptionEdit->toPlainText().isEmpty())
    {
        descriptionEdit->setPlainText(moduleTemplate.getDescription());
    }

    if (parametersEdit->toPlainText().isEmpty() && !moduleTemplate.getParameters().isEmpty())
    {
        parametersEdit->setPlainText(moduleTemplate.getParameters());
    }

    if (environmentEdit->toPlainText().isEmpty() && !moduleTemplate.getEnvironment().isEmpty())
    {
        environmentEdit->setPlainText(moduleTemplate.getEnvironment());
    }
}