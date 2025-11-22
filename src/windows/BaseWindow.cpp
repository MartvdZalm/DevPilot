#include "BaseWindow.h"
#include <QObject>

BaseWindow::BaseWindow(QWidget* parent) : QWidget(parent)
{
    QObject::connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this](Theme) { applyTheme(); });
}

BaseWindow::~BaseWindow() {}
