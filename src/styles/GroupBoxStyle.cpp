#include "GroupBoxStyle.h"

QString GroupBoxStyle::primary()
{
    return QString(R"(
        QGroupBox {
            font-weight: bold;
            border: 1px solid #3a3f47;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 10px;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
    )");
}