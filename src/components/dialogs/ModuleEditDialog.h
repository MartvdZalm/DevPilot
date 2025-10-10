#ifndef MODULEEDITDIALOG_H
#define MODULEEDITDIALOG_H

#include "../../models/Module.h"
#include "../../models/ModuleTemplate.h"
#include "../../models/Project.h"
#include "../../repositories/IModuleRepository.h"
#include "../../repositories/IModuleTemplateRepository.h"
#include "../../repositories/RepositoryProvider.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>

class ModuleEditDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ModuleEditDialog(const Module& module, RepositoryProvider& repositoryProvider, Project project,
                              QWidget* parent = nullptr);

    Module getModule();
    void setModule(const Module& module);

  private slots:
    void onOkClicked();
    void onCancelClicked();
    void onTemplateSelected(const QString& templateName);
    void onBrowseClicked();

  private:
    void setupUI();
    void setupConnections();
    void loadTemplatesFromRepository();
    void applyTemplate(const ModuleTemplate& moduleTemplate);
    void setupServiceTypeComboBox();

  private:
    Project project;
    Module module;
    IModuleRepository& moduleRepository;
    IModuleTemplateRepository& moduleTemplateRepository;

    QLineEdit* nameEdit;
    QSpinBox* portSpinBox;
    QLineEdit* commandEdit;
    QLineEdit* workingDirEdit;
    QPushButton* browseButton;
    QTextEdit* descriptionEdit;
    QTextEdit* parametersEdit;
    QTextEdit* environmentEdit;
    QCheckBox* autoStartCheckBox;
    QComboBox* templateComboBox;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QMap<QString, ModuleTemplate> moduleTemplateMap;
    QComboBox* serviceTypeComboBox;
};

#endif // MODULEEDITDIALOG_H
