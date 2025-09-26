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

    auto& db = Database::instance().database();

    auto projectRepository = std::make_shared<ProjectRepository>(db);
    auto noteRepository = std::make_shared<NoteRepository>(db);
    auto moduleRepository = std::make_shared<ModuleRepository>(db);
    auto settingRepository = std::make_shared<SettingRepository>(db);

    auto repositoryProvider = std::make_shared<RepositoryProvider>(
        projectRepository,
        noteRepository,
        moduleRepository,
        settingRepository
    );

    MainWindow window(repositoryProvider);
    window.setWindowTitle("DevPilot");
    window.showMaximized();

    return app.exec();
}
