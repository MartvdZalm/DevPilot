#include "core/Logger.h"
#include "database/Database.h"
#include "styles/AppStyle.h"
#include "windows/MainWindow.h"
#include "repositories/ProjectRepository.h"
#include "repositories/NoteRepository.h"
#include "repositories/ModuleRepository.h"
#include "repositories/SettingRepository.h"
#include "repositories/RepositoryProvider.h"

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

    QSqlDatabase& db = Database::instance().database();

    auto repositoryProvider = std::make_unique<RepositoryProvider>(
        std::make_unique<ProjectRepository>(db), std::make_unique<NoteRepository>(db),
        std::make_unique<ModuleRepository>(db), std::make_unique<SettingRepository>(db)
    );

    MainWindow window(*repositoryProvider);
    window.setWindowTitle("DevPilot");
    window.showMaximized();

    return app.exec();
}
