#include "FontStyle.h"
#include "ThemeManager.h"

QString FontStyle::h1()
{
    return h1(ThemeManager::instance().getCurrentTheme());
}

QString FontStyle::h2()
{
    return h2(ThemeManager::instance().getCurrentTheme());
}

QString FontStyle::h3()
{
    return h3(ThemeManager::instance().getCurrentTheme());
}

QString FontStyle::text()
{
    return text(ThemeManager::instance().getCurrentTheme());
}

QString FontStyle::textMuted()
{
    return textMuted(ThemeManager::instance().getCurrentTheme());
}

QString FontStyle::h1(Theme theme)
{
    if (theme == Theme::Light)
    {
        return "color: #212121; font-size: 20px; font-weight: bold;";
    }
    return "color: #ffffff; font-size: 20px; font-weight: bold;";
}

QString FontStyle::h2(Theme theme)
{
    if (theme == Theme::Light)
    {
        return "color: #212121; font-size: 16px; font-weight: bold;";
    }
    return "color: #ffffff; font-size: 16px; font-weight: bold;";
}

QString FontStyle::h3(Theme theme)
{
    if (theme == Theme::Light)
    {
        return "color: #424242; font-size: 14px; font-weight: bold;";
    }
    return "color: #c9d1d9; font-size: 14px; font-weight: bold;";
}

QString FontStyle::text(Theme theme)
{
    if (theme == Theme::Light)
    {
        return "color: #424242; font-size: 13px;";
    }
    return "color: #c9d1d9; font-size: 13px;";
}

QString FontStyle::textMuted(Theme theme)
{
    if (theme == Theme::Light)
    {
        return "color: #757575; font-size: 12px;";
    }
    return "color: #8b949e; font-size: 12px;";
}
