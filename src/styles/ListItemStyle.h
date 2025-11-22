#ifndef LISTITEMSTYLE_H
#define LISTITEMSTYLE_H

#include "ThemeManager.h"
#include <QString>

class ListItemStyle
{
  public:
    static QString primary();

    static QString primary(Theme theme);
};

#endif // LISTITEMSTYLE_H
