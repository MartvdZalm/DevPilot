#include "ButtonStyle.h"
#include "ThemeManager.h"

QString ButtonStyle::primary()
{
    auto& tm = ThemeManager::instance();

    return QString(R"(
        QPushButton, QToolButton {
            background-color: %1;
            border: 1px solid %2;
            color: %3;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            padding: 6px;
        }

        QPushButton:hover, QToolButton:hover {
            background-color: %4;
        }

        QPushButton:pressed, QToolButton:pressed {
            background-color: %5;
        }
    )")
        .arg(tm.color("button-bg"))
        .arg(tm.color("button-border"))
        .arg(tm.color("button-text"))
        .arg(tm.color("button-hover"))
        .arg(tm.color("button-pressed"));
}

QString ButtonStyle::icon()
{

    return icon(ThemeManager::instance().getCurrentTheme());
}

QString ButtonStyle::icon(Theme theme)
{
    if (theme == Theme::Light)
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
                background-color: rgba(0, 0, 0, 0.08);
                border-radius: 6px;
            }
            QPushButton:pressed, QToolButton:pressed {
                background-color: rgba(0, 0, 0, 0.15);
                border-radius: 6px;
            }

            QToolButton::menu-indicator {
                image: none;
            }
        )");
    }
    else
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
                background-color: rgba(255, 255, 255, 0.08);
                border-radius: 6px;
            }
            QPushButton:pressed, QToolButton:pressed {
                background-color: rgba(255, 255, 255, 0.15);
                border-radius: 6px;
            }

            QToolButton::menu-indicator {
                image: none;
            }
        )");
    }
}

QString ButtonStyle::success()
{
    return success(ThemeManager::instance().getCurrentTheme());
}

QString ButtonStyle::success(Theme theme)
{
    return QString(R"(
        QPushButton, QToolButton {
            background-color: #2e7d32;
            border: 1px solid #1b5e20;
            border-radius: 4px;
            color: #ffffff;
            font-size: 14px;
            font-weight: bold;
            padding: 6px 12px;
        }
        QPushButton:hover, QToolButton:hover {
            background-color: #43a047;
            border-color: #2e7d32;
        }
        QPushButton:pressed, QToolButton:pressed {
            background-color: #1b5e20;
            border-color: #0d3d10;
        }
        QPushButton:disabled, QToolButton:disabled {
            background-color: %1;
            border-color: %2;
            color: #bcbcbc;
        }
    )")
        .arg(theme == Theme::Light ? "#bdbdbd" : "#4e4e4e")
        .arg(theme == Theme::Light ? "#9e9e9e" : "#3e3e3e");
}

QString ButtonStyle::danger()
{
    return danger(ThemeManager::instance().getCurrentTheme());
}

QString ButtonStyle::danger(Theme theme)
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
            background-color: %1;
            border-color: %2;
            color: #bcbcbc;
        }
    )")
        .arg(theme == Theme::Light ? "#bdbdbd" : "#5d4037")
        .arg(theme == Theme::Light ? "#9e9e9e" : "#4e342e");
}
