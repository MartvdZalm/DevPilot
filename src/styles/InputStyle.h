#ifndef INPUTSTYLE_H
#define INPUTSTYLE_H

#include "ThemeManager.h"
#include <QString>

class InputStyle
{
  public:
    static QString primary();
    static QString commandLine();
    
    static QString primary(Theme theme);
    static QString commandLine(Theme theme);
};

#endif // INPUTSTYLE_H