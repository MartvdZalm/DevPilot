#include "ThemeManager.h"
#include <QSettings>

ThemeManager& ThemeManager::instance()
{
    static ThemeManager instance;
    static bool initialized = false;
    if (!initialized)
    {
        instance.loadTheme();
        initialized = true;
    }
    return instance;
}

Theme ThemeManager::getCurrentTheme() const
{
    return currentTheme;
}

void ThemeManager::setTheme(Theme theme)
{
    if (currentTheme != theme)
    {
        currentTheme = theme;
        loadColors();
        saveTheme();
        emit themeChanged(theme);
    }
}

QString ThemeManager::getThemeName() const
{
    return themeToString(currentTheme);
}

Theme ThemeManager::themeFromString(const QString& themeName)
{
    if (themeName.toLower() == "light")
    {
        return Theme::Light;
    }
    return Theme::Dark;
}

QString ThemeManager::themeToString(Theme theme)
{
    return theme == Theme::Light ? "light" : "dark";
}

void ThemeManager::loadTheme()
{
    QSettings settings;
    QString themeName = settings.value("theme", "dark").toString();
    currentTheme = themeFromString(themeName);
    loadColors();
}

void ThemeManager::saveTheme()
{
    QSettings settings;
    settings.setValue("theme", themeToString(currentTheme));
}

QString ThemeManager::color(const QString& key) const
{
    return currentTheme == Theme::Light ?
               lightColors.value(key) :
               darkColors.value(key);
}

void ThemeManager::loadColors()
{
    lightColors =
    {

        {"button-bg",        "#e0e0e0"},
        {"button-border",    "#bdbdbd"},
        {"button-hover",     "#d5d5d5"},
        {"button-pressed",   "#bdbdbd"},
        {"button-text",      "#212121"},

    };

    darkColors =
    {

        {"button-bg",        "#3a3f47"},
        {"button-border",    "#4a4a4a"},
        {"button-hover",     "#4a4f57"},
        {"button-pressed",   "#2c3137"},
        {"button-text",      "#ffffff"},

    };
}
