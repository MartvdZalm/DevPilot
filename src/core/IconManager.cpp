#include "IconManager.h"
#include "../styles/ThemeManager.h"
#include <QDebug>

IconManager& IconManager::instance()
{
    static IconManager instance;
    return instance;
}

IconManager::IconManager()
{
    setupIcons();
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &IconManager::updateAllIcons);
}

void IconManager::setupIcons()
{
    // Register all icons with their SVG paths
    icons["add"] = ThemedIcon(":/Icons/Add");
    icons["menu"] = ThemedIcon(":/Icons/Menu");
    // m_icons["save"] = ThemedIcon(":/Icons/save.svg");
    // m_icons["trash"] = ThemedIcon(":/Icons/trash.svg");
    // m_icons["copy"] = ThemedIcon(":/Icons/copy.svg");
    // m_icons["snippet"] = ThemedIcon(":/Icons/snippet.svg");
    // Add more icons as needed
}

QIcon IconManager::getIcon(const QString& iconName)
{
    if (icons.contains(iconName)) {
        return icons[iconName].icon();
    }
    return QIcon(); // Return empty icon if not found
}

void IconManager::updateAllIcons()
{
    for (auto& icon : icons)
    {
        icon.updateTheme();
    }
}
