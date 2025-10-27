#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include "../../models/Process.h"
#include "../../models/ProcessTemplate.h"
#include "../../models/Project.h"
#include "../../repositories/RepositoryProvider.h"
#include "../../repositories/interfaces/IProcessRepository.h"
#include "../../repositories/interfaces/IProcessTemplateRepository.h"

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

class ProcessDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ProcessDialog(const Process& process, RepositoryProvider& repositoryProvider, Project project,
                           QWidget* parent = nullptr);

    Process getProcess();
    void setProcess(const Process& process);

  private slots:
    void onOkClicked();
    void onCancelClicked();
    void onTemplateSelected(const QString& templateName);
    void onBrowseClicked();

  private:
    void setupUI();
    void setupConnections();
    void loadTemplatesFromRepository();
    void applyTemplate(const ProcessTemplate& processTemplate);

  private:
    Project project;
    Process process;
    IProcessRepository& processRepository;
    IProcessTemplateRepository& processTemplateRepository;

    QLineEdit* nameEdit;
    QSpinBox* portSpinBox;
    QLineEdit* commandEdit;
    QLineEdit* workingDirEdit;
    QPushButton* browseButton;
    QComboBox* templateComboBox;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QMap<QString, ProcessTemplate> processTemplateMap;
};

#endif // PROCESSDIALOG_H
