#include "ListStyle.h"
#include "ThemeManager.h"

QString ListStyle::primary()
{
    return primary(ThemeManager::instance().getCurrentTheme());
}

QString ListStyle::primary(Theme theme)
{
    if (theme == Theme::Light)
    {
        return QString(R"(
            QListWidget {
                background-color: transparent;
                border: none;
                color: #212121;
                font-size: 14px;
            }

            QListWidget:focus {
                outline: none;
            }

            QListWidget::item {
                padding: 8px;
                border-radius: 4px;
                margin: 2px 0px;
            }

            QListWidget::item:selected {
                background-color: #e3f2fd;
                color: #1976d2;
            }

            QListWidget::item:hover {
               background-color: #f5f5f5; 
            }
        )");
    }
    else
    {
        return QString(R"(
            QListWidget {
                background-color: transparent;
                border: none;
                color: #ffffff;
                font-size: 14px;
            }

            QListWidget:focus {
                outline: none;
            }

            QListWidget::item {
                padding: 8px;
                border-radius: 4px;
                margin: 2px 0px;
            }

            QListWidget::item:selected {
                background-color: #3a414a;
            }

            QListWidget::item:hover {
               background-color: #383f48; 
            }
        )");
    }
}
