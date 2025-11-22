#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include "../styles/ThemeManager.h"
#include <QWidget>

class BaseWindow : public QWidget
{
    Q_OBJECT

  public:
    explicit BaseWindow(QWidget* parent = nullptr);
    ~BaseWindow();

    virtual void setupUI() = 0;
    virtual void setupConnections() = 0;

  protected slots:
    // Called when theme changes to let derived windows reapply per-widget styles
    virtual void applyTheme() {}
};

#endif // BASEWINDOW_H