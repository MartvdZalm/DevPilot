#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QAction>
#include <QPoint>
#include <QToolButton>
#include <QWidget>

class TitleBar : public QWidget
{
    Q_OBJECT

  public:
    explicit TitleBar(QWidget* parent = nullptr);
    void refreshStyle();

  private:
    void setupUI();
    void setupMenu();
    void setupConnections();

  private:
    QToolButton* burgerButton = nullptr;
    QMenu* menu = nullptr;
    QAction* settingsAction = nullptr;
    QAction* snippetsAction = nullptr;
    QAction* exitAction = nullptr;
};

#endif // TITLEBAR_H
