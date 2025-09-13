#ifndef NOTEDIALOG_H
#define NOTEDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include "../../models/Note.h"

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

    QLineEdit* titleEdit;
    QTextEdit* contentEdit;

    Note currentNote;
};

#endif // NOTEDIALOG_H
