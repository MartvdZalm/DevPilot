#include "core/Logger.h"
#include "database/Database.h"
#include "database/seeders/ProcessTemplateSeeder.h"
#include "database/seeders/Seeder.h"
#include "repositories/AppRepository.h"
#include "repositories/EditorRepository.h"
#include "repositories/NoteRepository.h"
#include "repositories/ProcessRepository.h"
#include "repositories/ProcessTemplateRepository.h"
#include "repositories/ProjectRepository.h"
#include "repositories/RepositoryProvider.h"
#include "repositories/SnippetRepository.h"
#include "styles/AppStyle.h"
#include "styles/ThemeManager.h"
#include "windows/MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    ThemeManager::instance();

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
    auto processRepo = std::make_unique<ProcessRepository>(db);
    auto editorRepo = std::make_unique<EditorRepository>(db);
    auto processTemplateRepo = std::make_unique<ProcessTemplateRepository>(db);
    auto appRepo = std::make_unique<AppRepository>(db);
    auto snippetRepo = std::make_unique<SnippetRepository>(db);

    // Run seeders
    Seeder seeder;
    seeder.addSeeder(std::make_unique<ProcessTemplateSeeder>(*processTemplateRepo));

    if (!seeder.runSeeders())
    {
        LOG_WARNING("Some database seeders failed, but continuing application startup");
    }

    // Create repository provider with the repositories
    auto repositoryProvider = std::make_unique<RepositoryProvider>(
        std::move(projectRepo), std::move(noteRepo), std::move(processRepo), std::move(editorRepo),
        std::move(processTemplateRepo), std::move(appRepo), std::move(snippetRepo));

    MainWindow window(*repositoryProvider);
    window.setWindowTitle("DevPilot");
    window.showMaximized();

    // Connect theme changes to update application stylesheet
    QObject::connect(&ThemeManager::instance(), &ThemeManager::themeChanged, &app,
                     [&app](Theme theme) { app.setStyleSheet(AppStyle::styleSheet(theme)); });

    return app.exec();
}
