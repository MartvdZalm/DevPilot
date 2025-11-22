#ifndef FONTSTYLE_H
#define FONTSTYLE_H

#include "ThemeManager.h"
#include <QString>

class FontStyle
{
  public:
    static QString h1();
    static QString h2();
    static QString h3();
    static QString text();
    static QString textMuted();

    static QString h1(Theme theme);
    static QString h2(Theme theme);
    static QString h3(Theme theme);
    static QString text(Theme theme);
    static QString textMuted(Theme theme);

};

#endif // FONTSTYLE_H
