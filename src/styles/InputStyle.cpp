#include "InputStyle.h"

QString InputStyle::primary()
{
    return primary(ThemeManager::instance().getCurrentTheme());
}

QString InputStyle::primary(Theme theme)
{
    if (theme == Theme::Light)
    {
        return QString(R"(
            QLineEdit, QSpinBox, QTextEdit, QComboBox {
               background-color: #ffffff;
               border: 1px solid #e0e0e0;
               color: #212121;
               padding: 4px;
               border-radius: 4px;
            }
        )");
    }
    else
    {
        return QString(R"(
            QLineEdit, QSpinBox, QTextEdit, QComboBox {
               background-color: #2f343b;
               border: 1px solid #4a4a4a;
               color: #ffffff;
               padding: 4px;
               border-radius: 4px;
            }
        )");
    }
}

QString InputStyle::commandLine()
{
    return commandLine(ThemeManager::instance().getCurrentTheme());
}

QString InputStyle::commandLine(Theme theme)
{
    if (theme == Theme::Light)
    {
        return QString(R"(
            QTextEdit {
               background-color: #f5f5f5;
               border: 1px solid #e0e0e0;
               border-radius: 4px;
               color: #212121;
               font-size: 11px;
               padding: 8px;
            }
        )");
    }
    else
    {
        return QString(R"(
            QTextEdit {
               background-color: #1e1e1e;
               border: 1px solid #3a3f47;
               border-radius: 4px;
               color: #ffffff;
               font-size: 11px;
               padding: 8px;
            }
        )");
    }
}
