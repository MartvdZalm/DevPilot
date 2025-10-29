#include "SettingsWindow.h"

#include "../styles/ButtonStyle.h"
#include "../styles/ListStyle.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QSettings>

SettingsWindow::SettingsWindow(RepositoryProvider& repoProvider, QWidget* parent)
    : editorRepository(repoProvider.getEditorRepository()),
      processTemplateRepository(repoProvider.getProcessTemplateRepository()),
      appRepository(repoProvider.getAppRepository()), BaseWindow(parent)
{
    setFixedSize(QSize(1000, 600));
    setWindowTitle("Settings");
    setupUI();
    setupConnections();
    loadSettings();
}

void SettingsWindow::setupUI()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setupSidebar();
    setupContentArea();
    setupButtonArea();

    QWidget* rightContainer = new QWidget;
    rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    rightLayout->addWidget(contentFrame, 1);
    rightLayout->addWidget(buttonFrame, 0);

    mainLayout->addWidget(sidebarFrame, 0);
    mainLayout->addWidget(rightContainer, 1);

    sidebar->setCurrentRow(0);
    onSidebarItemChanged(0);
}

void SettingsWindow::setupConnections()
{
    connect(sidebar, &QListWidget::currentRowChanged, this, &SettingsWindow::onSidebarItemChanged);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsWindow::onCancelClicked);
    connect(applyButton, &QPushButton::clicked, this, &SettingsWindow::onApplyClicked);
    connect(addEditorButton, &QPushButton::clicked, this, &SettingsWindow::onAddEditorClicked);
    connect(editorsTable, &QTableWidget::cellChanged, this, &SettingsWindow::onEditorRowChanged);
    connect(addTemplateButton, &QPushButton::clicked, this, &SettingsWindow::onAddTemplateClicked);
    connect(templatesTable, &QTableWidget::cellChanged, this, &SettingsWindow::onTemplateRowChanged);
}

void SettingsWindow::setupSidebar()
{
    sidebarFrame = new QFrame;
    sidebarFrame->setFixedWidth(200);
    sidebarFrame->setFrameStyle(QFrame::StyledPanel);

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebarFrame);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    sidebar = new QListWidget;
    sidebar->setFrameStyle(QFrame::NoFrame);
    sidebar->setStyleSheet(ListStyle::primary());

    sidebar->addItem("General");
    sidebar->addItem("Editors");
    sidebar->addItem("Process Templates");
    sidebar->addItem("Apps");
    sidebar->addItem("About");

    sidebarLayout->addWidget(sidebar);
}

void SettingsWindow::setupContentArea()
{
    contentFrame = new QFrame;
    contentFrame->setFrameStyle(QFrame::StyledPanel);

    QVBoxLayout* contentLayout = new QVBoxLayout(contentFrame);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    contentStack = new QStackedWidget;

    generalPage = createGeneralPage();
    editorsPage = createEditorsPage();
    templatesPage = createTemplatesPage();
    aboutPage = createAboutPage();
    appsPage = createAppsPage();

    contentStack->addWidget(generalPage);
    contentStack->addWidget(editorsPage);
    contentStack->addWidget(templatesPage);
    contentStack->addWidget(appsPage);
    contentStack->addWidget(aboutPage);

    scrollArea->setWidget(contentStack);
    contentLayout->addWidget(scrollArea);
}

void SettingsWindow::setupButtonArea()
{
    buttonFrame = new QFrame;
    buttonFrame->setFixedHeight(60);
    buttonFrame->setFrameStyle(QFrame::StyledPanel);

    buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setContentsMargins(15, 10, 15, 10);
    buttonLayout->addStretch();

    cancelButton = new QPushButton("Cancel");
    cancelButton->setStyleSheet(ButtonStyle::primary());

    applyButton = new QPushButton("Apply");
    applyButton->setStyleSheet(ButtonStyle::primary());

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(applyButton);
}

QWidget* SettingsWindow::createGeneralPage()
{
    QWidget* page = new QWidget;
    return page;
}

QWidget* SettingsWindow::createEditorsPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("Configure IDEs and Editors");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 15px;");
    layout->addWidget(titleLabel);

    QLabel* descriptionLabel = new QLabel("Configure the editors you want to appear in the 'Open in IDE' menu.");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("color: #666; margin-bottom: 15px;");
    layout->addWidget(descriptionLabel);

    QHBoxLayout* buttonLayout = new QHBoxLayout;

    addEditorButton = new QPushButton("Add Editor");
    addEditorButton->setStyleSheet(ButtonStyle::primary());

    buttonLayout->addWidget(addEditorButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    editorsTable = new QTableWidget;
    editorsTable->setColumnCount(5);
    editorsTable->setHorizontalHeaderLabels({"Enabled", "Name", "Path", "Arguments", "Actions"});
    editorsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    editorsTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    editorsTable->setColumnWidth(0, 80);
    editorsTable->setColumnWidth(1, 150);
    editorsTable->setColumnWidth(2, 250);
    editorsTable->setColumnWidth(3, 150);

    layout->addWidget(editorsTable);

    return page;
}

QWidget* SettingsWindow::createTemplatesPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("Manage Process Templates");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 15px;");
    layout->addWidget(titleLabel);

    QLabel* descriptionLabel =
        new QLabel("Configure custom process templates that appear when creating new processes. "
                   "Templates automatically pre-fill commands, ports, and settings for common project types.");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("color: #666; margin-bottom: 15px;");
    layout->addWidget(descriptionLabel);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    addTemplateButton = new QPushButton("Add Template");
    addTemplateButton->setStyleSheet(ButtonStyle::primary());
    buttonLayout->addWidget(addTemplateButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    templatesTable = new QTableWidget;
    templatesTable->setColumnCount(6);
    templatesTable->setHorizontalHeaderLabels({"Enabled", "Name", "Command", "Port", "Description", "Actions"});
    templatesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    templatesTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    templatesTable->setColumnWidth(0, 80);
    templatesTable->setColumnWidth(1, 120);
    templatesTable->setColumnWidth(2, 150);
    templatesTable->setColumnWidth(3, 80);
    templatesTable->setColumnWidth(4, 200);

    layout->addWidget(templatesTable);

    return page;
}

QWidget* SettingsWindow::createAppsPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("Manage Applications");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 15px;");
    layout->addWidget(titleLabel);

    QLabel* descriptionLabel = new QLabel("Configure the applications that can be linked to your projects.");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("color: #666; margin-bottom: 15px;");
    layout->addWidget(descriptionLabel);

    QHBoxLayout* buttonLayout = new QHBoxLayout;

    addAppButton = new QPushButton("Add App");
    addAppButton->setStyleSheet(ButtonStyle::primary());
    connect(addAppButton, &QPushButton::clicked, this,
            [this]()
            {
                addAppRow();
                applyButton->setEnabled(true);
            });

    buttonLayout->addWidget(addAppButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    appsTable = new QTableWidget;
    appsTable->setColumnCount(5);
    appsTable->setHorizontalHeaderLabels({"Enabled", "Name", "Path", "Arguments", "Actions"});
    appsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    appsTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    appsTable->setColumnWidth(0, 80);
    appsTable->setColumnWidth(1, 150);
    appsTable->setColumnWidth(2, 250);
    appsTable->setColumnWidth(3, 150);

    connect(appsTable, &QTableWidget::cellChanged, this, [this]() { applyButton->setEnabled(true); });

    layout->addWidget(appsTable);
    return page;
}

void SettingsWindow::addAppRow(const App& app)
{
    int row = appsTable->rowCount();
    appsTable->insertRow(row);

    QCheckBox* enabledCheck = new QCheckBox;
    enabledCheck->setChecked(app.isEnabled());
    enabledCheck->setStyleSheet("margin-left: 20px;");
    appsTable->setCellWidget(row, 0, enabledCheck);
    connect(enabledCheck, &QCheckBox::stateChanged, this, [this]() { applyButton->setEnabled(true); });

    QTableWidgetItem* nameItem = new QTableWidgetItem(app.getName());
    appsTable->setItem(row, 1, nameItem);

    QTableWidgetItem* pathItem = new QTableWidgetItem(app.getPath());
    appsTable->setItem(row, 2, pathItem);

    QTableWidgetItem* argsItem = new QTableWidgetItem(app.getArguments());
    appsTable->setItem(row, 3, argsItem);

    QPushButton* deleteButton = new QPushButton("Delete");
    deleteButton->setStyleSheet(ButtonStyle::danger());

    connect(deleteButton, &QPushButton::clicked, this,
            [this, row]()
            {
                QMessageBox::StandardButton reply =
                    QMessageBox::question(this, "Delete App", "Are you sure you want to delete this app?",
                                          QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                    appsTable->removeRow(row);
                    applyButton->setEnabled(true);
                }
            });

    appsTable->setCellWidget(row, 4, deleteButton);
}

void SettingsWindow::loadApps()
{
    appsTable->setRowCount(0);
    currentApps = appRepository.findAll();

    for (const App& app : currentApps)
    {
        addAppRow(app);
    }
}

void SettingsWindow::saveApps()
{
    QList<App> appsToSave;

    for (int row = 0; row < appsTable->rowCount(); ++row)
    {
        App app;

        QCheckBox* enabledCheck = static_cast<QCheckBox*>(appsTable->cellWidget(row, 0));
        if (enabledCheck)
        {
            app.setEnabled(enabledCheck->isChecked());
        }

        QTableWidgetItem* nameItem = appsTable->item(row, 1);
        if (!nameItem || nameItem->text().isEmpty())
        {
            continue;
        }
        app.setName(nameItem->text());

        QTableWidgetItem* pathItem = appsTable->item(row, 2);
        if (pathItem)
        {
            app.setPath(pathItem->text());
        }

        QTableWidgetItem* argsItem = appsTable->item(row, 3);
        if (argsItem)
        {
            app.setArguments(argsItem->text());
        }

        appsToSave.append(app);
    }

    for (const App& existing : appRepository.findAll())
    {
        appRepository.deleteById(existing.getId());
    }

    for (App& app : appsToSave)
    {
        appRepository.save(app);
    }

    currentApps = appsToSave;
}

QWidget* SettingsWindow::createAboutPage()
{
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* placeholder = new QLabel("About..");
    layout->addWidget(placeholder);
    layout->addStretch();

    return page;
}

void SettingsWindow::onSidebarItemChanged(int index)
{
    contentStack->setCurrentIndex(index);
}

void SettingsWindow::onCancelClicked()
{
    loadSettings();
}

void SettingsWindow::onApplyClicked()
{
    saveSettings();
}

void SettingsWindow::onAddEditorClicked()
{
    addEditorRow();
}

void SettingsWindow::onEditorRowChanged(int row, int column)
{
    applyButton->setEnabled(true);
}

void SettingsWindow::addEditorRow(const Editor& editor)
{
    int row = editorsTable->rowCount();
    editorsTable->insertRow(row);

    QCheckBox* enabledCheck = new QCheckBox;
    enabledCheck->setChecked(editor.isEnabled());
    enabledCheck->setStyleSheet("margin-left: 20px;");
    editorsTable->setCellWidget(row, 0, enabledCheck);

    connect(enabledCheck, &QCheckBox::stateChanged, this, [this]() { applyButton->setEnabled(true); });

    QTableWidgetItem* nameItem = new QTableWidgetItem(editor.getName());
    editorsTable->setItem(row, 1, nameItem);

    QTableWidgetItem* pathItem = new QTableWidgetItem(editor.getPath());
    editorsTable->setItem(row, 2, pathItem);

    QTableWidgetItem* argsItem = new QTableWidgetItem(editor.getArguments());
    editorsTable->setItem(row, 3, argsItem);

    QPushButton* deleteButton = new QPushButton("Delete");
    deleteButton->setStyleSheet(ButtonStyle::danger());
    deleteButton->setProperty("row", row);

    connect(deleteButton, &QPushButton::clicked, this,
            [this, row]()
            {
                QMessageBox::StandardButton reply =
                    QMessageBox::question(this, "Delete Editor", "Are you sure you want to delete this editor?",
                                          QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                    editorsTable->removeRow(row);
                    applyButton->setEnabled(true);
                }
            });

    editorsTable->setCellWidget(row, 4, deleteButton);
}

void SettingsWindow::loadSettings()
{
    QSettings settings;
    loadEditors();
    loadTemplates();
    loadApps();
    applyButton->setEnabled(false);
}

void SettingsWindow::saveSettings()
{
    QSettings settings;
    saveEditors();
    saveTemplates();
    saveApps();
    applyButton->setEnabled(false);
}

void SettingsWindow::loadEditors()
{
    editorsTable->setRowCount(0);
    currentEditors = editorRepository.findAll();

    for (const Editor& editor : currentEditors)
    {
        addEditorRow(editor);
    }
}

void SettingsWindow::saveEditors()
{
    QList<Editor> editorsToSave;

    for (int row = 0; row < editorsTable->rowCount(); ++row)
    {
        Editor editor;

        QCheckBox* enabledCheck = static_cast<QCheckBox*>(editorsTable->cellWidget(row, 0));
        if (enabledCheck)
        {
            editor.setEnabled(enabledCheck->isChecked());
        }

        QTableWidgetItem* nameItem = editorsTable->item(row, 1);
        if (nameItem && !nameItem->text().isEmpty())
        {
            editor.setName(nameItem->text());
        }
        else
        {
            continue;
        }

        QTableWidgetItem* pathItem = editorsTable->item(row, 2);
        if (pathItem)
        {
            editor.setPath(pathItem->text());
        }

        QTableWidgetItem* argsItem = editorsTable->item(row, 3);
        if (argsItem)
        {
            editor.setArguments(argsItem->text());
        }

        editor.setDisplayOrder(row);
        editorsToSave.append(editor);
    }

    QList<Editor> existingEditors = editorRepository.findAll();
    for (const Editor& existing : existingEditors)
    {
        editorRepository.deleteById(existing.getId());
    }

    for (Editor& editor : editorsToSave)
    {
        editorRepository.save(editor);
    }

    currentEditors = editorsToSave;
}

void SettingsWindow::addTemplateRow(const ProcessTemplate& processTemplate)
{
    int row = templatesTable->rowCount();
    templatesTable->insertRow(row);

    QCheckBox* enabledCheck = new QCheckBox;
    enabledCheck->setChecked(processTemplate.isEnabled());
    enabledCheck->setStyleSheet("margin-left: 20px;");
    templatesTable->setCellWidget(row, 0, enabledCheck);
    connect(enabledCheck, &QCheckBox::stateChanged, this, [this]() { applyButton->setEnabled(true); });

    QTableWidgetItem* nameItem = new QTableWidgetItem(processTemplate.getName());
    templatesTable->setItem(row, 1, nameItem);

    QTableWidgetItem* commandItem = new QTableWidgetItem(processTemplate.getCommand());
    templatesTable->setItem(row, 2, commandItem);

    QTableWidgetItem* portItem = new QTableWidgetItem(QString::number(processTemplate.getPort()));
    templatesTable->setItem(row, 3, portItem);

    QTableWidgetItem* descriptionItem = new QTableWidgetItem(processTemplate.getDescription());
    templatesTable->setItem(row, 4, descriptionItem);

    QPushButton* deleteButton = new QPushButton("Delete");
    deleteButton->setStyleSheet(ButtonStyle::danger());
    deleteButton->setProperty("row", row);

    connect(deleteButton, &QPushButton::clicked, this,
            [this, row]()
            {
                QMessageBox::StandardButton reply =
                    QMessageBox::question(this, "Delete Template", "Are you sure you want to delete this template?",
                                          QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                    templatesTable->removeRow(row);
                    applyButton->setEnabled(true);
                }
            });

    templatesTable->setCellWidget(row, 5, deleteButton);
}

void SettingsWindow::loadTemplates()
{
    templatesTable->setRowCount(0);
    currentTemplates = processTemplateRepository.findAll();

    for (const ProcessTemplate& processTemplate : currentTemplates)
    {
        addTemplateRow(processTemplate);
    }
}

void SettingsWindow::saveTemplates()
{
    QList<ProcessTemplate> templatesToSave;

    for (int row = 0; row < templatesTable->rowCount(); ++row)
    {
        ProcessTemplate processTemplate;

        QCheckBox* enabledCheck = static_cast<QCheckBox*>(templatesTable->cellWidget(row, 0));
        if (enabledCheck)
        {
            processTemplate.setEnabled(enabledCheck->isChecked());
        }

        QTableWidgetItem* nameItem = templatesTable->item(row, 1);
        if (nameItem && !nameItem->text().isEmpty())
        {
            processTemplate.setName(nameItem->text());
        }
        else
        {
            continue;
        }

        QTableWidgetItem* commandItem = templatesTable->item(row, 2);
        if (commandItem)
        {
            processTemplate.setCommand(commandItem->text());
        }

        QTableWidgetItem* portItem = templatesTable->item(row, 3);
        if (portItem && !portItem->text().isEmpty())
        {
            processTemplate.setPort(portItem->text().toInt());
        }

        QTableWidgetItem* descriptionItem = templatesTable->item(row, 4);
        if (descriptionItem)
        {
            processTemplate.setDescription(descriptionItem->text());
        }

        templatesToSave.append(processTemplate);
    }

    QList<ProcessTemplate> existingTemplates = processTemplateRepository.findAll();
    for (const ProcessTemplate& existing : existingTemplates)
    {
        processTemplateRepository.deleteById(existing.getId());
    }

    for (ProcessTemplate& processTemplate : templatesToSave)
    {
        processTemplateRepository.save(processTemplate);
    }

    currentTemplates = templatesToSave;
}

void SettingsWindow::onAddTemplateClicked()
{
    ProcessTemplate newTemplate;
    addTemplateRow(newTemplate);
    applyButton->setEnabled(true);
}

void SettingsWindow::onTemplateRowChanged(int row, int column)
{
    applyButton->setEnabled(true);
}
