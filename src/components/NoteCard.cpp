#include "NoteCard.h"
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>

NoteCard::NoteCard(const Note& note, QWidget* parent) : QFrame(parent), note(note)
{
    setObjectName("NoteCard");
    setFixedSize(170, 150);
    setCursor(Qt::PointingHandCursor);

    setStyleSheet(
        "QFrame#NoteCard {"
        "  background: #FFF9C4;"
        "  border: 1px solid #FBC02D;"
        "  border-radius: 8px;"
        "}"
        "QFrame#NoteCard:hover {"
        "  background: #FFF59D;"
        "}"
        );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(4);

    titleLabel = new QLabel(note.getTitle(), this);
    titleLabel->setStyleSheet("background: transparent; color: #212121; font-weight: bold; font-size: 16px;");

    descLabel = new QLabel(note.getContent(), this);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("background: transparent; color: #555555; font-size: 11px;");
    descLabel->setAlignment(Qt::AlignTop);

    QHBoxLayout* bottomRow = new QHBoxLayout();
    bottomRow->setContentsMargins(0, 0, 0, 0);

    deleteBtn = new QPushButton(this);
    deleteBtn->setIcon(QIcon(":/Images/Bin"));
    deleteBtn->setIconSize(QSize(20, 20));
    deleteBtn->setFixedSize(24, 24);
    deleteBtn->setStyleSheet(
        "QPushButton { background: transparent; border: none; }"
        "QPushButton:hover { color: red; }"
        );

    connect(deleteBtn, &QPushButton::clicked, this, [this, note]() { emit deleteClicked(note); });

    bottomRow->addStretch();
    bottomRow->addWidget(deleteBtn, 0, Qt::AlignRight);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(descLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomRow);
}


void NoteCard::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked(note);
    }
    QFrame::mousePressEvent(event);
}
