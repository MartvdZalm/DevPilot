#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "BaseWindow.h"
#include "../repositories/RepositoryProvider.h"
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
    explicit SettingsWindow(std::shared_ptr<RepositoryProvider> repoRepository, QWidget* parent = nullptr);

  private slots:
    void onSidebarItemChanged(int index);
    void onCancelClicked();
    void onApplyClicked();
    
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

  private:
    std::shared_ptr<RepositoryProvider> repositoryRepository;

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
    QWidget* aboutPage;
};

#endif // SETTINGSWINDOW_H
