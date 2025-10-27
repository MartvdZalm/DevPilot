#ifndef APP_EVENTS_H
#define APP_EVENTS_H

#include <QObject>

class AppEvents : public QObject
{
    Q_OBJECT

  public:
    static AppEvents& instance();

    void notifyNavigateToSettings();
    void notifyNavigateToSnippets();

  signals:
    void navigateToSettings();
    void navigateToSnippets();

};

#endif // APP_EVENTS_H
