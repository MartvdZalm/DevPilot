#ifndef THEMEDICON_H
#define THEMEDICON_H

#include <QIcon>
#include <QString>
#include "../styles/ThemeManager.h"

class ThemedIcon
{
  public:
    ThemedIcon() = default;
    ThemedIcon(const QString& svgPath);
    QIcon icon() const;
    QIcon icon(Theme theme) const;
    void updateTheme();

  private:
    QString svgPath;
    mutable QIcon darkIcon;
    mutable QIcon lightIcon;

    QIcon createIcon(Theme theme) const;
    QString loadAndColorSvg(Theme theme) const;
};

#endif // THEMEDICON_H
