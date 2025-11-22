#ifndef TABLESTYLE_H
#define TABLESTYLE_H

#include "ThemeManager.h"
#include <QString>

class TableStyle
{
  public:
    static QString primary();
    static QString primary(Theme theme);
};

#endif // TABLESTYLE_H
