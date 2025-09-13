#include "InputStyle.h"

QString InputStyle::primary()
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

QString InputStyle::commandLine()
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
