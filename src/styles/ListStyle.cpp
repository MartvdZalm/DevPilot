#include "ListStyle.h"

QString ListStyle::primary()
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
