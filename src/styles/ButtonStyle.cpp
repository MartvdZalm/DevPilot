#include "ButtonStyle.h"

QString ButtonStyle::primary()
{
    return QString(R"(
        QPushButton, QToolButton {
            background-color: #3a3f47;
            border: 1px solid #4a4a4a;
            border-radius: 12px;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 6px;
            border-radius: 4px;
        }

        QPushButton:hover, QToolButton:hover {
            background-color: #4a4f57;
        }

        QPushButton:pressed, QToolButton:pressed {
            background-color: #2c3137;
        }
    )");
}

QString ButtonStyle::icon()
{
    return QString(R"(
        QPushButton, QToolButton {
            background: transparent;
            border: none;
            padding: 4px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover, QToolButton:hover {
            background-color: rgba(255, 255, 255, 0.08); /* soft hover */
            border-radius: 6px;
        }
        QPushButton:pressed, QToolButton:pressed {
            background-color: rgba(255, 255, 255, 0.15); /* stronger press */
            border-radius: 6px;
        }
    )");
}
