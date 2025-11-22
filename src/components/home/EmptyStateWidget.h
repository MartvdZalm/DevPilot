#ifndef EMPTYSTATEWIDGET_H
#define EMPTYSTATEWIDGET_H

#include <QPushButton>
#include <QWidget>
#include <QLabel>

class EmptyStateWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit EmptyStateWidget(QWidget* parent = nullptr);
    void refreshStyle();

  signals:
    void createProjectClicked();

  private:
    void setupUI();
    void setupConnections();

  private:
    QLabel* emptyTitle = nullptr;
    QLabel* emptySubtitle = nullptr;
    QPushButton* createProjectButton = nullptr;
};
;

#endif // EMPTYSTATEWIDGET_H
