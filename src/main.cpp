#include "core/Logger.h"
#include "database/Database.h"
#include "repositories/EditorRepository.h"
#include "repositories/ModuleRepository.h"
#include "repositories/ModuleTemplateRepository.h"
#include "repositories/NoteRepository.h"
#include "repositories/ProjectRepository.h"
#include "repositories/AppRepository.h"
#include "repositories/RepositoryProvider.h"
#include "database/seeders/ModuleTemplateSeeder.h"
#include "database/seeders/Seeder.h"
#include "styles/AppStyle.h"
#include "windows/MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(AppStyle::styleSheet());
    app.setWindowIcon(QIcon(":/Images/AppIcon"));

    Logger::initialize("DevPilot");

    if (!Database::instance().initialize())
    {
        LOG_CRITICAL("Failed to initialize database");
        return -1;
    }

    // Create repositories first
    QSqlDatabase& db = Database::instance().getDatabase();

    auto projectRepo = std::make_unique<ProjectRepository>(db);
    auto noteRepo = std::make_unique<NoteRepository>(db);
    auto moduleRepo = std::make_unique<ModuleRepository>(db);
    auto editorRepo = std::make_unique<EditorRepository>(db);
    auto moduleTemplateRepo = std::make_unique<ModuleTemplateRepository>(db);
    auto appRepo = std::make_unique<AppRepository>(db);

    // Run seeders
    Seeder seeder;
    seeder.addSeeder(std::make_unique<ModuleTemplateSeeder>(*moduleTemplateRepo));

    if (!seeder.runSeeders())
    {
        LOG_WARNING("Some database seeders failed, but continuing application startup");
    }

    // Create repository provider with the repositories
    auto repositoryProvider = std::make_unique<RepositoryProvider>(
        std::move(projectRepo), std::move(noteRepo), std::move(moduleRepo),
        std::move(editorRepo), std::move(moduleTemplateRepo), std::move(appRepo));

    MainWindow window(*repositoryProvider);
    window.setWindowTitle("DevPilot");
    window.showMaximized();

    return app.exec();
}
