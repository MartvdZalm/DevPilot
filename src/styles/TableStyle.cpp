#include "TableStyle.h"
#include "ThemeManager.h"

QString TableStyle::primary()
{
    return primary(ThemeManager::instance().getCurrentTheme());
}

QString TableStyle::primary(Theme theme)
{
    if (theme == Theme::Light)
    {
        return QString(R"(
        /* =============================
           QTableWidget / QTableView
        ============================= */
        QTableWidget, QTableView {
            background-color: #ffffff;
            alternate-background-color: #f5f5f5;
            border: 1px solid #e0e0e0;
            gridline-color: #e0e0e0;
            color: #212121;
            font-family: 'JetBrains Mono', monospace;
            font-size: 13px;
            selection-background-color: #e3f2fd;
            selection-color: #1976d2;
        }

        QTableCornerButton::section {
            background-color: #ffffff;
            border: 1px solid #e0e0e0;
        }

        /* =============================
           Headers (compact IntelliJ-like)
        ============================= */
        QHeaderView {
            background-color: #f5f5f5;
            border: none;
        }

        QHeaderView::section {
            background-color: #f5f5f5;
            color: #424242;
            border: 1px solid #e0e0e0;
            padding: 2px 6px;   /* Tight padding */
            font-weight: normal;
            font-size: 12px;
        }

        QHeaderView::section:horizontal {
            border-top: none;
            border-left: none;
            border-right: 1px solid #e0e0e0;
            border-bottom: 1px solid #e0e0e0;
        }

        QHeaderView::section:vertical {
            border-left: none;
            border-top: none;
            border-right: 1px solid #e0e0e0;
            border-bottom: 1px solid #e0e0e0;
        }

        /* =============================
           Table items
        ============================= */
        QTableWidget::item {
            padding: 3px 6px;
            border: none;
        }

        QTableWidget::item:selected {
            background-color: #e3f2fd;
            color: #1976d2;
        }

        /* Hover highlight like IntelliJ */
        QTableView::item:hover {
            background-color: #f5f5f5;
        }

        /* =============================
           ScrollBars (match AppStyle)
        ============================= */
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c0c0c0, stop:1 #d0d0d0);
            border-radius: 7px;
            min-height: 30px;
            margin: 2px;
        }

        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #a0a0a0, stop:1 #b0b0b0);
        }

        QScrollBar::handle:vertical:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #909090, stop:1 #a0a0a0);
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
            border: none;
        }

        QScrollBar:horizontal {
            background: #f5f5f5;
            height: 14px;
            margin: 0px;
            border: none;
            border-radius: 7px;
        }

        QScrollBar::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #c0c0c0, stop:1 #d0d0d0);
            border-radius: 7px;
            min-width: 30px;
            margin: 2px;
        }

        QScrollBar::handle:horizontal:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #a0a0a0, stop:1 #b0b0b0);
        }

        QScrollBar::handle:horizontal:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #909090, stop:1 #a0a0a0);
        }

        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal,
        QScrollBar::sub-page:horizontal {
            background: none;
            border: none;
        }
    )");
    }
    else // Dark theme
    {
        return QString(R"(
        /* =============================
           QTableWidget / QTableView
        ============================= */
        QTableWidget, QTableView {
            background-color: #2f343b;
            alternate-background-color: #333840;
            border: 1px solid #4a4a4a;
            gridline-color: #4a4a4a;
            color: #ffffff;
            font-family: 'JetBrains Mono', monospace;
            font-size: 13px;
            selection-background-color: #3a414a;
            selection-color: #ffffff;
        }

        QTableCornerButton::section {
            background-color: #2f343b;
            border: 1px solid #4a4a4a;
        }

        /* =============================
           Headers (compact IntelliJ-like)
        ============================= */
        QHeaderView {
            background-color: #2a2d33;
            border: none;
        }

        QHeaderView::section {
            background-color: #2a2d33;
            color: #d0d0d0;
            border: 1px solid #3a3f46;
            padding: 2px 6px;   /* Tight padding */
            font-weight: normal;
            font-size: 12px;
        }

        QHeaderView::section:horizontal {
            border-top: none;
            border-left: none;
            border-right: 1px solid #3a3f46;
            border-bottom: 1px solid #3a3f46;
        }

        QHeaderView::section:vertical {
            border-left: none;
            border-top: none;
            border-right: 1px solid #3a3f46;
            border-bottom: 1px solid #3a3f46;
        }

        /* =============================
           Table items
        ============================= */
        QTableWidget::item {
            padding: 3px 6px;
            border: none;
        }

        QTableWidget::item:selected {
            background-color: #3a414a;
            color: #ffffff;
        }

        /* Hover highlight like IntelliJ */
        QTableView::item:hover {
            background-color: #383f48;
        }

        /* =============================
           ScrollBars (match AppStyle)
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
    )");
    }
}
