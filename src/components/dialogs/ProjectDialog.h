#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include "../../models/Project.h"
#include "../../models/App.h"
#include "../../repositories/RepositoryProvider.h"
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

class ProjectDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ProjectDialog(RepositoryProvider& repoProvider, QWidget* parent = nullptr, const Project& projectToEdit = Project());

    Project getProject() const;
    QList<int> getSelectedAppIds() const;

  private slots:
    void onOkClicked();
    void onCancelClicked();

  private:
    void setupUI();
    void setupConnections();
    void populateFields();
    void loadApps();

  private:
    Project project;
    bool editing = false;

    QLineEdit* nameEdit;
    QLineEdit* pathEdit;
    QTextEdit* descriptionEdit;
    QListWidget* appListWidget;

    QPushButton* okButton;
    QPushButton* cancelButton;
    QPushButton* browseButton;

    RepositoryProvider& repositoryProvider;
    QList<App> allApps;
};

#endif // PROJECTDIALOG_H
