#ifndef APP_EVENTS_H
#define APP_EVENTS_H

#include <QObject>

class AppEvents : public QObject
{
	Q_OBJECT

	public:
    static AppEvents& instance();

    void notifyNavigateToSettings();

signals:
    void navigateToSettings();

};

#endif // APP_EVENTS_H
