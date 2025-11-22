#ifndef APPSTYLE_H
#define APPSTYLE_H

#include "ThemeManager.h"
#include <QString>

class AppStyle
{
  public:
    static QString styleSheet();
    static QString styleSheet(Theme theme);
};

#endif // APPSTYLE_H