#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include "../../models/Project.h"
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

class ProjectDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ProjectDialog(QWidget* parent = nullptr, const Project& projectToEdit = Project());

    Project getProject() const;

  private slots:
    void onOkClicked();
    void onCancelClicked();

  private:
    void setupUI();
    void setupConnections();
    void populateFields();

  private:
    Project project;
    bool editing = false;
    QLineEdit* nameEdit;
    QLineEdit* pathEdit;
    QTextEdit* descriptionEdit;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QPushButton* browseButton;
};

#endif // PROJECTDIALOG_H
