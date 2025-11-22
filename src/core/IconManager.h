#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include "ThemedIcon.h"
#include <QString>
#include <QMap>

class IconManager : public QObject
{
    Q_OBJECT

  public:
    static IconManager& instance();

    QIcon getIcon(const QString& iconName);
    void updateAllIcons();

    QIcon add() { return getIcon("add"); }
    QIcon menu() { return getIcon("menu"); }
    QIcon save() { return getIcon("save"); }
    QIcon trash() { return getIcon("trash"); }
    QIcon copy() { return getIcon("copy"); }
    QIcon snippet() { return getIcon("snippet"); }

  private:
    IconManager();
    void setupIcons();

    QMap<QString, ThemedIcon> icons;
};

#endif // ICONMANAGER_H
