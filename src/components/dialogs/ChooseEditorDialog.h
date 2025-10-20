#ifndef CHOOSEEDITORDIALOG_H
#define CHOOSEEDITORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QListWidget>
#include "../../models/Editor.h"

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
