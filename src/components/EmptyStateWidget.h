#ifndef EMPTYSTATEWIDGET_H
#define EMPTYSTATEWIDGET_H

#include <QPushButton>
#include <QWidget>

class EmptyStateWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit EmptyStateWidget(QWidget* parent = nullptr);

    QPushButton* getCreateProjectButton() const
    {
        return createProjectButton;
    }

  signals:
    void createProjectClicked();

  private:
    void setupUI();

  private:
    QPushButton* createProjectButton = nullptr;
};
;

#endif // EMPTYSTATEWIDGET_H
