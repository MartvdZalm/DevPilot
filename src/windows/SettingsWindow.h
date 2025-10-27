#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "../repositories/RepositoryProvider.h"
#include "BaseWindow.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QTableWidget>
#include <QWidget>

class SettingsWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit SettingsWindow(RepositoryProvider& repoRepository, QWidget* parent = nullptr);

  private slots:
    void onSidebarItemChanged(int index);
    void onCancelClicked();
    void onApplyClicked();
    void onAddEditorClicked();
    void onEditorRowChanged(int row, int column);
    void onAddTemplateClicked();
    void onTemplateRowChanged(int row, int column);

  private:
    void setupUI() override;
    void setupConnections() override;
    void setupSidebar();
    void setupContentArea();
    void setupButtonArea();
    void loadSettings();
    void saveSettings();

    QWidget* createGeneralPage();
    QWidget* createAboutPage();

    QWidget* createEditorsPage();
    void addEditorRow(const Editor& editor = Editor());
    void loadEditors();
    void saveEditors();

    QWidget* createTemplatesPage();
    void addTemplateRow(const ProcessTemplate& processTemplate = ProcessTemplate());
    void loadTemplates();
    void saveTemplates();

    QWidget* createAppsPage();
    void addAppRow(const App& app = App());
    void loadApps();
    void saveApps();

  private:
    IEditorRepository& editorRepository;
    IProcessTemplateRepository& processTemplateRepository;
    IAppRepository& appRepository;

    QHBoxLayout* mainLayout;
    QVBoxLayout* leftLayout;
    QVBoxLayout* rightLayout;
    QListWidget* sidebar;
    QFrame* sidebarFrame;
    QStackedWidget* contentStack;
    QScrollArea* scrollArea;
    QFrame* contentFrame;
    QFrame* buttonFrame;
    QHBoxLayout* buttonLayout;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QPushButton* applyButton;

    QWidget* aboutPage;

    // Editors Page Widgets
    QWidget* editorsPage;
    QTableWidget* editorsTable;
    QPushButton* addEditorButton;
    QList<Editor> currentEditors;

    // Templates Page Widgets
    QWidget* templatesPage;
    QTableWidget* templatesTable;
    QPushButton* addTemplateButton;
    QList<ProcessTemplate> currentTemplates;

    // Apps Page Widgets
    QWidget* appsPage;
    QTableWidget* appsTable;
    QPushButton* addAppButton;
    QList<App> currentApps;

    // General Page Widgets
    QWidget* generalPage;
    QCheckBox* startupCheckbox;
    QCheckBox* minimizeToTrayCheckbox;
    QCheckBox* autoStartCheckbox;
    QCheckBox* alwaysOnTopCheckbox;
    QComboBox* themeComboBox;
    QLineEdit* defaultLocationLineEdit;
};

#endif // SETTINGSWINDOW_H
