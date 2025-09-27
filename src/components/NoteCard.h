#ifndef NOTECARD_H
#define NOTECARD_H

#include "../models/Note.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class NoteCard : public QFrame
{
    Q_OBJECT

  public:
    explicit NoteCard(const Note& note, QWidget* parent = nullptr);

  signals:
    void clicked(const Note& note);
    void deleteClicked(const Note& note);

  protected:
    void mousePressEvent(QMouseEvent* event) override;

  private:
    void setupUI();

  private:
    Note note;
    QLabel* titleLabel = nullptr;
    QLabel* descLabel = nullptr;
    QVBoxLayout* layout = nullptr;
    QPushButton* deleteBtn = nullptr;
};

#endif // NOTECARD_H
