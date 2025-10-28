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

    void notifyRefreshHomeSidebar();

  signals:
    void navigateToSettings();
    void navigateToSnippets();

    void refreshHomeSidebar();
};

#endif // APP_EVENTS_H
