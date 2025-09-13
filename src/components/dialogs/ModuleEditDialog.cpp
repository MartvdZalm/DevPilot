#include "ModuleEditDialog.h"
#include "../../styles/ButtonStyle.h"
#include "../../styles/GroupBoxStyle.h"
#include "../../styles/InputStyle.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

ModuleEditDialog::ModuleEditDialog(QWidget* parent) : QDialog(parent)
{
    setupUI();
    setupConnections();
}

ModuleEditDialog::ModuleEditDialog(const Module& module, QWidget* parent) : QDialog(parent), module(module)
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

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("e.g., Spring Boot API, Vue Frontend");
    nameEdit->setStyleSheet(InputStyle::primary());
    basicLayout->addRow("Name:", nameEdit);

    portSpinBox = new QSpinBox();
    portSpinBox->setRange(1, 65535);
    portSpinBox->setValue(3000);
    portSpinBox->setStyleSheet(InputStyle::primary());
    basicLayout->addRow("Port:", portSpinBox);

    descriptionEdit = new QTextEdit();
    descriptionEdit->setMaximumHeight(60);
    descriptionEdit->setPlaceholderText("Describe what this module does...");
    descriptionEdit->setStyleSheet(InputStyle::primary());
    basicLayout->addRow("Description:", descriptionEdit);

    mainLayout->addWidget(basicGroup);

    QGroupBox* execGroup = new QGroupBox("Execution Settings");
    execGroup->setStyleSheet(GroupBoxStyle::primary());
    QFormLayout* execLayout = new QFormLayout(execGroup);

    commandEdit = new QLineEdit();
    commandEdit->setPlaceholderText("e.g., npm start, mvn spring-boot:run, python app.py");
    commandEdit->setStyleSheet(InputStyle::primary());
    execLayout->addRow("Command:", commandEdit);

    workingDirEdit = new QLineEdit();
    workingDirEdit->setPlaceholderText("Working directory (leave empty for project root)");
    workingDirEdit->setStyleSheet(InputStyle::primary());
    execLayout->addRow("Working Directory:", workingDirEdit);

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

    // Buttons
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
    autoStartCheckBox->setChecked(module.getAutoStart());
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
    module.setAutoStart(autoStartCheckBox->isChecked());

    accept();
}

void ModuleEditDialog::onCancelClicked()
{
    reject();
}
