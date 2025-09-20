#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "BaseWindow.h"
#include <QHBoxLayout>
#include <QListWidget>
#include <QFrame>
#include <QWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include <QPushButton>

class SettingsWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit SettingsWindow(QWidget* parent = nullptr);

    void setupUI() override;
    void setupConnections() override;

  private slots:
    void onSidebarItemChanged(int index);
    void onCancelClicked();
    void onApplyClicked();
    
  private:
    void setupSidebar();
    void setupContentArea();
    void setupButtonArea();
    void loadSettings();
    void saveSettings();

    QWidget* createGeneralPage();
    QWidget* createAudioPage();
    QWidget* createLibraryPage();
    QWidget* createPlaybackPage();
    QWidget* createInterfacePage();
    QWidget* createHotkeysPage();
    QWidget* createAdvancedPage();
    QWidget* createAboutPage();

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

    QWidget* generalPage;
    QWidget* audioPage;
    QWidget* libraryPage;
    QWidget* playbackPage;
    QWidget* interfacePage;
    QWidget* hotkeysPage;
    QWidget* advancedPage;
    QWidget* aboutPage;
};

#endif // SETTINGSWINDOW_H
