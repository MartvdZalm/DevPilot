#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPoint>
#include <QToolButton>
#include <QAction>

class TitleBar : public QWidget
{
    Q_OBJECT

  public:
    explicit TitleBar(QWidget* parent = nullptr);

  private:
    void setupUI();
    void setupMenu();
    void setupConnections();

  private:
    QToolButton* burgerButton = nullptr;
    QMenu* menu = nullptr;
    QAction* settingsAction = nullptr;
    QAction* exitAction = nullptr;
};

#endif // TITLEBAR_H
