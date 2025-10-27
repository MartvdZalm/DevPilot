#ifndef NOTEDIALOG_H
#define NOTEDIALOG_H

#include "../../models/Note.h"
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>

class NoteDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit NoteDialog(QWidget* parent = nullptr);
    explicit NoteDialog(const Note& note, QWidget* parent = nullptr);

    Note getNote() const;

  private slots:
    void onOkClicked();
    void onCancelClicked();

  private:
    void setupUI();
    void loadNote(const Note& note);

  private:
    QLineEdit* titleEdit;
    QTextEdit* contentEdit;
    Note currentNote;
};

#endif // NOTEDIALOG_H
