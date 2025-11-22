#ifndef BUTTONSTYLE_H
#define BUTTONSTYLE_H

#include "ThemeManager.h"
#include <QString>

class ButtonStyle
{
  public:
    static QString primary();
    static QString success();
    static QString danger();
    static QString icon();
    
    static QString success(Theme theme);
    static QString danger(Theme theme);
    static QString icon(Theme theme);
};

#endif // BUTTONSTYLE_H
