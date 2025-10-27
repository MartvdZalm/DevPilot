#ifndef CHOOSEEDITORDIALOG_H
#define CHOOSEEDITORDIALOG_H

#include "../../models/Editor.h"
#include <QDialog>
#include <QListWidget>
#include <QPushButton>

class ChooseEditorDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ChooseEditorDialog(const QList<Editor>& editors, QWidget* parent = nullptr);
    Editor getSelectedEditor();

  private:
    void setupUI();

  private:
    QPushButton* okButton;
    QPushButton* cancelButton;
    const QList<Editor>& editors;
    QListWidget* editorList;
};

#endif // CHOOSEEDITORDIALOG_H
