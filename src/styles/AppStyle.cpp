#include "AppStyle.h"

QString AppStyle::styleSheet()
{
    return QString(R"(
        /* =============================
           Global Background & Text
        ============================= */
        QWidget {
            background-color: #2a2d33;
            color: #ffffff;
            font-family: 'JetBrains Mono', monospace;
        }

        /* =============================
           ScrollBars (your original)
        ============================= */
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4a4a4a, stop:1 #5a5a5a);
            border-radius: 7px;
            min-height: 30px;
            margin: 2px;
        }

        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #6a6a6a, stop:1 #7a7a7a);
        }

        QScrollBar::handle:vertical:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3a3a3a, stop:1 #4a4a4a);
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
            border: none;
        }

        QScrollBar:horizontal {
            background: #1e1e1e;
            height: 14px;
            margin: 0px;
            border: none;
            border-radius: 7px;
        }

        QScrollBar::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4a4a4a, stop:1 #5a5a5a);
            border-radius: 7px;
            min-width: 30px;
            margin: 2px;
        }

        QScrollBar::handle:horizontal:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6a6a6a, stop:1 #7a7a7a);
        }

        QScrollBar::handle:horizontal:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3a3a3a, stop:1 #4a4a4a);
        }

        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal,
        QScrollBar::sub-page:horizontal {
            background: none;
            border: none;
        }

        /* =============================
           TreeWidget / ListWidget
        ============================= */
        QTreeWidget, QListWidget {
            background-color: #2f343b;
            border: none;
            color: #ffffff;
        }

        QTreeWidget::item:selected, QListWidget::item:selected {
            background-color: #3a414a;
            color: #ffffff;
        }

        QTreeWidget::item:hover, QListWidget::item:hover {
            background-color: #383f48;
        }

        /* =============================
           TabWidget
        ============================= */
        QTabWidget::pane {
            border: 1px solid #4a4a4a;
            background: #2f343b;
        }

        QTabBar::tab {
            background: #3a3f47;
            border: 1px solid #4a4a4a;
            padding: 6px 12px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }

        QTabBar::tab:selected {
            background: #282d33;
            border-bottom-color: #282d33;
        }
    )");
}
