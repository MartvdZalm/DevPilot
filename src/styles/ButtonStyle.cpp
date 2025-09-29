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

QString ButtonStyle::danger()
{
    return QString(R"(
        QPushButton, QToolButton {
            background-color: #d32f2f;
            border: 1px solid #b71c1c;
            border-radius: 4px;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 6px 12px;
        }

        QPushButton:hover, QToolButton:hover {
            background-color: #f44336;
            border-color: #c62828;
        }

        QPushButton:pressed, QToolButton:pressed {
            background-color: #b71c1c;
            border-color: #8e0e0e;
        }

        QPushButton:disabled, QToolButton:disabled {
            background-color: #5d4037;
            border-color: #4e342e;
            color: #bcbcbc;
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

        QToolButton::menu-indicator {
            image: none;
        }
    )");
}
