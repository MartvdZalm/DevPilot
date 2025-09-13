#include "core/Logger.h"
#include "database/Container.h"
#include "database/Database.h"
#include "styles/AppStyle.h"
#include "windows/MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(AppStyle::styleSheet());
    Logger::initialize("DevPilot");

    if (!Database::instance().initialize())
    {
        qFatal("Failed to initialize database");
        return -1;
    }

    Container::instance().initialize(Database::instance().database());

    MainWindow window;
    window.setWindowTitle("DevPilot");
    window.showMaximized();

    return app.exec();
}
