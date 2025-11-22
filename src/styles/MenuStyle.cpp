#include "MenuStyle.h"
#include "ThemeManager.h"

QString MenuStyle::primary()
{
    return primary(ThemeManager::instance().getCurrentTheme());
}

QString MenuStyle::primary(Theme theme)
{
    return QString(R"(
        QMenu {
            background-color: #2f343b;
            border: 1px solid #4a4a4a;
            border-radius: 6px;
            padding: 4px;
        }

        QMenu::item {
            padding: 6px 12px;
            color: #ffffff;
            background-color: transparent;
            border-radius: 4px;
        }

        QMenu::item:selected {
            background-color: #4a4f57;
        }

        QMenu::item:pressed {
            background-color: #2c3137;
        }

        QMenu::separator {
            height: 1px;
            background: #4a4a4a;
            margin: 4px 6px;
        }
    )");
}
