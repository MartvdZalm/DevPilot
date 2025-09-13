#ifndef MODULEEDITDIALOG_H
#define MODULEEDITDIALOG_H

#include "../../models/Module.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>

class ModuleEditDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ModuleEditDialog(QWidget* parent = nullptr);
    explicit ModuleEditDialog(const Module& module, QWidget* parent = nullptr);

    Module getModule();
    void setModule(const Module& module);

  private slots:
    void onOkClicked();
    void onCancelClicked();

  private:
    void setupUI();
    void setupConnections();

    Module module;

    QLineEdit* nameEdit;
    QSpinBox* portSpinBox;
    QLineEdit* commandEdit;
    QLineEdit* workingDirEdit;
    QTextEdit* descriptionEdit;
    QTextEdit* parametersEdit;
    QTextEdit* environmentEdit;
    QCheckBox* autoStartCheckBox;

    QPushButton* okButton;
    QPushButton* cancelButton;
};

#endif // MODULEEDITDIALOG_H
