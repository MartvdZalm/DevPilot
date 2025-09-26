#include "TitleBar.h"

#include "../styles/ButtonStyle.h"
#include "../styles/MenuStyle.h"
#include "../events/AppEvents.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QMenu>
#include <QMouseEvent>
#include <QIcon>
#include <QApplication>

TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(32);
    setupUI();
    setupMenu();
    setupConnections();
}

void TitleBar::setupUI()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(10);

    burgerButton = new QToolButton(this);
    burgerButton->setStyleSheet(ButtonStyle::icon());
    burgerButton->setIcon(QIcon(":/Images/Menu"));
    burgerButton->setIconSize(QSize(25, 25));
    burgerButton->setPopupMode(QToolButton::InstantPopup);

    layout->addWidget(burgerButton);
    layout->addStretch();

    setLayout(layout);
}

void TitleBar::setupMenu()
{
    QMenu* menu = new QMenu(this);
    menu->setStyleSheet(MenuStyle::primary());

    settingsAction = menu->addAction("Settings");
    menu->addSeparator();
    exitAction = menu->addAction("Exit");

    burgerButton->setMenu(menu);
}

void TitleBar::setupConnections()
{
    connect(settingsAction, &QAction::triggered, this, [this] { AppEvents::instance().notifyNavigateToSettings(); });
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
}
