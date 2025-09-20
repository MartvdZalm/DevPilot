#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPoint>

class TitleBar : public QWidget
{
    Q_OBJECT

  public:
    explicit TitleBar(QWidget* parent = nullptr);
};

#endif // TITLEBAR_H
