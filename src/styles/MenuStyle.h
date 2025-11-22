#ifndef MENUSTYLE_H
#define MENUSTYLE_H

#include "Theme.h"
#include <QString>

class MenuStyle
{
  public:
    static QString primary();

    static QString primary(Theme theme);
};

#endif // MENUSTYLE_H
