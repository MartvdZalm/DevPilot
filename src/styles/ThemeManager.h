#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include "Theme.h"
#include <QObject>
#include <QString>
#include <QHash>

class ThemeManager : public QObject
{
    Q_OBJECT

  public:
    static ThemeManager& instance();

    Theme getCurrentTheme() const;
    void setTheme(Theme theme);
    QString getThemeName() const;

    static Theme themeFromString(const QString& themeName);
    static QString themeToString(Theme theme);
    QString color(const QString& key) const;

  signals:
    void themeChanged(Theme theme);

  private:
    ThemeManager() = default;
    ~ThemeManager() = default;
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    Theme currentTheme = Theme::Dark;
    void loadTheme();
    void saveTheme();

    QHash<QString, QString> lightColors;
    QHash<QString, QString> darkColors;

    void loadColors();
};

#endif // THEMEMANAGER_H
