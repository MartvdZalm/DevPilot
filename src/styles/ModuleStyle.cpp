#include "ModuleStyle.h"

QString ModuleStyle::header()
{
    return QString(R"(
        QWidget {
            background-color: #1e1e1e;
            border-radius: 8px;
            padding: 10px;
        }
    )");
}

QString ModuleStyle::statusIndicator(bool running)
{
    return QString("color: %1; font-size: 12px;").arg(running ? "#7ee787" : "#f85149");
}

QString ModuleStyle::tabWidget()
{
    return QString(R"(
        QTabWidget::pane {
            border: 1px solid #4a4a4a;
            background-color: #2f343b;
            border-radius: 6px;
        }
        QTabBar::tab {
            background-color: #3a3f47;
            color: #a0a0a0;
            padding: 8px 14px;
            margin-right: 2px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
        }
        QTabBar::tab:selected {
            background-color: #282d33;
            color: #ffffff;
        }
        QTabBar::tab:hover {
            background-color: #383f48;
        }
    )");
}

QString ModuleStyle::terminal()
{
    return QString(R"(
        QTextEdit {
            background-color: #0d1117;
            color: #c9d1d9;
            font-family: 'JetBrains Mono', 'Consolas', monospace;
            font-size: 12px;
            border: none;
            padding: 10px;
            border-radius: 4px;
        }
    )");
}

QString ModuleStyle::commandInput()
{
    return QString(R"(
        QLineEdit {
            background-color: transparent;
            color: #c9d1d9;
            border: none;
            font-family: 'JetBrains Mono', 'Consolas', monospace;
            font-size: 12px;
            padding: 4px;
        }
        QLineEdit:disabled {
            color: #666666;
        }
    )");
}

QString ModuleStyle::inputContainer()
{
    return QString(R"(
        QWidget {
            background-color: #0d1117;
            border-radius: 8px;
            padding: 8px;
        }
    )");
}

QString ModuleStyle::sectionLabel()
{
    return QString(R"(
        QLabel {
            color: #ffffff;
            font-weight: bold;
            margin-bottom: 5px;
        }
    )");
}

QString ModuleStyle::infoValue()
{
    return QString(R"(
        QLabel {
            color: #a0a0a0;
            background-color: #0d1117;
            border-radius: 4px;
            padding: 8px;
        }
    )");
}
