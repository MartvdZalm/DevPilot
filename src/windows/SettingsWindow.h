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

class SettingsWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit SettingsWindow(RepositoryProvider& repoRepository, QWidget* parent = nullptr);

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
    RepositoryProvider& repositoryRepository;

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
