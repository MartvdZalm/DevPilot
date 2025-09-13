#include "NoteDialog.h"
#include "../../styles/InputStyle.h"
#include "../../styles/ButtonStyle.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

NoteDialog::NoteDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
}

NoteDialog::NoteDialog(const Note& note, QWidget* parent)
    : QDialog(parent), currentNote(note)
{
    setupUI();
    loadNote(note);
}

void NoteDialog::setupUI()
{
    setWindowTitle("Note");
    resize(400, 300);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Title:");
    titleEdit = new QLineEdit();
    titleEdit->setStyleSheet(InputStyle::primary());

    QLabel* contentLabel = new QLabel("Content:");
    contentEdit = new QTextEdit();
    contentEdit->setStyleSheet(InputStyle::primary());

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setStyleSheet(ButtonStyle::primary());
    QPushButton* okButton = new QPushButton("OK");
    okButton->setStyleSheet(ButtonStyle::primary());

    connect(okButton, &QPushButton::clicked, this, &NoteDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &NoteDialog::onCancelClicked);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(titleEdit);
    mainLayout->addWidget(contentLabel);
    mainLayout->addWidget(contentEdit);

    mainLayout->addLayout(buttonLayout);
}

void NoteDialog::loadNote(const Note& note)
{
    titleEdit->setText(note.getTitle());
    contentEdit->setPlainText(note.getContent());
}

Note NoteDialog::getNote() const
{
    Note note = currentNote;
    note.setTitle(titleEdit->text());
    note.setContent(contentEdit->toPlainText());
    return note;
}

void NoteDialog::onOkClicked()
{
    if (titleEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Note title is required!");
        return;
    }

    if (contentEdit->toPlainText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Validation Error", "Content is required!");
        return;
    }

    currentNote.setTitle(titleEdit->text().trimmed());
    currentNote.setContent(contentEdit->toPlainText().trimmed());

    accept();
}

void NoteDialog::onCancelClicked()
{
    reject();
}
