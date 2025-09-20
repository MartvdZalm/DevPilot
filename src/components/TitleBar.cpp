#include "TitleBar.h"

#include "../styles/ButtonStyle.h"
#include "../styles/MenuStyle.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QMenu>
#include <QMouseEvent>
#include <QIcon>

TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(32);

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(10);

    QToolButton* burgerButton = new QToolButton(this);
    burgerButton->setStyleSheet(ButtonStyle::icon());
    burgerButton->setIcon(QIcon(":/Images/Menu"));
    burgerButton->setIconSize(QSize(25, 25));
    burgerButton->setPopupMode(QToolButton::InstantPopup);

    QMenu* menu = new QMenu(this);
    menu->setStyleSheet(MenuStyle::primary());
    menu->addAction("Home");
    menu->addAction("Settings");
    menu->addSeparator();
    menu->addAction("Exit");
    burgerButton->setMenu(menu);

    layout->addWidget(burgerButton);
    layout->addStretch();

    setLayout(layout);
}
