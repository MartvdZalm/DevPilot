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
    : editorRepository(repoProvider.getEditorRepository()), BaseWindow(parent)
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
    aboutPage = createAboutPage();

    contentStack->addWidget(generalPage);
    contentStack->addWidget(editorsPage);
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
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(20);

    QGroupBox* behaviorGroup = new QGroupBox("Application Behavior");
    behaviorGroup->setStyleSheet("QGroupBox { font-weight: bold; color: #ffffff; }");

    QVBoxLayout* behaviorLayout = new QVBoxLayout(behaviorGroup);

    startupCheckbox = new QCheckBox("Open last project on startup");
    startupCheckbox->setStyleSheet("color: #ffffff;");
    behaviorLayout->addWidget(startupCheckbox);

    minimizeToTrayCheckbox = new QCheckBox("Minimize to system tray");
    minimizeToTrayCheckbox->setStyleSheet("color: #ffffff;");
    behaviorLayout->addWidget(minimizeToTrayCheckbox);

    autoStartCheckbox = new QCheckBox("Start with Windows");
    autoStartCheckbox->setStyleSheet("color: #ffffff;");
    behaviorLayout->addWidget(autoStartCheckbox);

    behaviorLayout->addSpacing(10);

    QLabel* windowBehaviorLabel = new QLabel("Window Behavior:");
    windowBehaviorLabel->setStyleSheet("color: #a0a0a0; font-weight: bold; margin-top: 10px;");
    behaviorLayout->addWidget(windowBehaviorLabel);

    alwaysOnTopCheckbox = new QCheckBox("Always on top");
    alwaysOnTopCheckbox->setStyleSheet("color: #ffffff;");
    behaviorLayout->addWidget(alwaysOnTopCheckbox);

    behaviorLayout->addStretch();
    layout->addWidget(behaviorGroup);

    QGroupBox* projectGroup = new QGroupBox("Project Settings");
    projectGroup->setStyleSheet("QGroupBox { font-weight: bold; color: #ffffff; }");

    QFormLayout* projectLayout = new QFormLayout(projectGroup);
    projectLayout->setLabelAlignment(Qt::AlignRight);

    QLabel* defaultLocationLabel = new QLabel("Default project location:");
    defaultLocationLabel->setStyleSheet("color: #ffffff;");
    QHBoxLayout* locationLayout = new QHBoxLayout();
    defaultLocationLineEdit = new QLineEdit();
    defaultLocationLineEdit->setStyleSheet(ButtonStyle::primary());
    defaultLocationLineEdit->setPlaceholderText("C:/Projects/");
    QPushButton* browseButton = new QPushButton("Browse");
    browseButton->setStyleSheet(ButtonStyle::primary());
    locationLayout->addWidget(defaultLocationLineEdit);
    locationLayout->addWidget(browseButton);
    projectLayout->addRow(defaultLocationLabel, locationLayout);

    layout->addWidget(projectGroup);

    QGroupBox* uiGroup = new QGroupBox("Interface Settings");
    uiGroup->setStyleSheet("QGroupBox { font-weight: bold; color: #ffffff; }");

    QVBoxLayout* uiLayout = new QVBoxLayout(uiGroup);

    QLabel* themeLabel = new QLabel("Theme:");
    themeLabel->setStyleSheet("color: #ffffff;");
    uiLayout->addWidget(themeLabel);

    themeComboBox = new QComboBox();
    themeComboBox->setStyleSheet(ButtonStyle::primary());
    themeComboBox->addItems({"Dark", "Light", "System"});
    uiLayout->addWidget(themeComboBox);

    uiLayout->addStretch();
    layout->addWidget(uiGroup);

    layout->addStretch();

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

    startupCheckbox->setChecked(settings.value("behavior/openLastProject", true).toBool());
    minimizeToTrayCheckbox->setChecked(settings.value("behavior/minimizeToTray", false).toBool());
    autoStartCheckbox->setChecked(settings.value("behavior/startWithWindows", false).toBool());
    alwaysOnTopCheckbox->setChecked(settings.value("behavior/alwaysOnTop", false).toBool());

    defaultLocationLineEdit->setText(settings.value("project/defaultLocation", "").toString());

    QString theme = settings.value("interface/theme", "Dark").toString();
    int index = themeComboBox->findText(theme);
    if (index >= 0)
        themeComboBox->setCurrentIndex(index);
    

    loadEditors();
    applyButton->setEnabled(false);
}

void SettingsWindow::saveSettings()
{
    QSettings settings;

    settings.setValue("behavior/openLastProject", startupCheckbox->isChecked());
    settings.setValue("behavior/minimizeToTray", minimizeToTrayCheckbox->isChecked());
    settings.setValue("behavior/startWithWindows", autoStartCheckbox->isChecked());
    settings.setValue("behavior/alwaysOnTop", alwaysOnTopCheckbox->isChecked());
    settings.setValue("project/defaultLocation", defaultLocationLineEdit->text());
    settings.setValue("interface/theme", themeComboBox->currentText());
    
    saveEditors();
    applyButton->setEnabled(false);
    QMessageBox::information(this, "Settings Saved", "Editor settings have been saved successfully.");
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