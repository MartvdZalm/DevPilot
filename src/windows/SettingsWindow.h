#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "../repositories/RepositoryProvider.h"
#include "BaseWindow.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QWidget>
#include <QTableWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

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

  private:
    void setupUI() override;
    void setupConnections() override;
    void setupSidebar();
    void setupContentArea();
    void setupButtonArea();
    void loadSettings();
    void saveSettings();

    QWidget* createGeneralPage();
    QWidget* createEditorsPage();
    QWidget* createAboutPage();

    void addEditorRow(const Editor& editor = Editor());
    void loadEditors();
    void saveEditors();

  private:
    IEditorRepository& editorRepository;

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
