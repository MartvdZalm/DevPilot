#include "ListItemStyle.h"

QString ListItemStyle::primary()
{
    return primary(ThemeManager::instance().getCurrentTheme());
}

QString ListItemStyle::primary(Theme theme)
{
    if (theme == Theme::Light)
    {
        return QString(R"(

        )");
    }
    else
    {
        return QString(R"(
            QTreeWidget::item:selected, QListWidget::item:selected {
                background-color: #e3f2fd;
                color: #1976d2;
            }

            QTreeWidget::item:hover, QListWidget::item:hover {
                background-color: #f5f5f5;
            }
        )");
    }
}
