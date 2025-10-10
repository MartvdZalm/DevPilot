#ifndef MODULESTYLE_H
#define MODULESTYLE_H

#include <QString>

class ModuleStyle
{
  public:
    static QString header();
    static QString statusIndicator(bool running);
    static QString tabWidget();
    static QString terminal();
    static QString commandInput();
    static QString inputContainer();
    static QString sectionLabel();
    static QString infoValue();
};

#endif // MODULESTYLE_H
