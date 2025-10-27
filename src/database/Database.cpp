#include "Database.h"
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

Database& Database::instance()
{
    static Database instance;
    return instance;
}

Database::~Database()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool Database::initialize()
{
    if (initialized)
        return true;

    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/DevPilot.db";
    QDir().mkpath(QFileInfo(dbPath).absolutePath());

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open())
    {
        qCritical() << "Database error:" << db.lastError();
        return false;
    }

    if (!applyPragmas() || !createTables() || !verifyDatabase())
    {
        qCritical() << "Database initialization failed.";
        return false;
    }

    initialized = true;
    return true;
}

bool Database::applyPragmas()
{
    QSqlQuery query(db);
    return query.exec("PRAGMA foreign_keys = ON") && query.exec("PRAGMA journal_mode = WAL") &&
           query.exec("PRAGMA synchronous = NORMAL") && query.exec("PRAGMA temp_store = MEMORY");
}

bool Database::createTables()
{
    QSqlQuery query(db);

    if (!query.exec("BEGIN IMMEDIATE TRANSACTION"))
    {
        qCritical() << "Failed to begin transaction:" << query.lastError();
        return false;
    }

    bool success = createProjectsTable(query) && createProcessesTable(query) && createNotesTable(query) &&
                   createEditorsTable(query) && createProcessTemplatesTable(query) && createAppsTables(query) &&
                   createSnippetTable(query);

    if (success)
        query.exec("COMMIT");
    else
    {
        qCritical() << "Rolling back due to error.";
        query.exec("ROLLBACK");
    }

    return success;
}

bool Database::createProjectsTable(QSqlQuery& query)
{
    const QString sql = R"(
        CREATE TABLE IF NOT EXISTS projects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            directory_path TEXT NOT NULL,
            description TEXT,
            created_at DATETIME NOT NULL,
            updated_at DATETIME NOT NULL,
            UNIQUE(name, directory_path)
        )
    )";

    return query.exec(sql);
}

bool Database::createProcessesTable(QSqlQuery& query)
{
    const QString sql = R"(
        CREATE TABLE IF NOT EXISTS processes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            project_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            command TEXT NOT NULL,
            working_directory TEXT NOT NULL,
            status TEXT NOT NULL DEFAULT 'stopped',
            pid INTEGER,
            port INTEGER,
            log_path TEXT,
            last_started_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            uptime DATETIME,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (project_id) REFERENCES projects (id) ON DELETE CASCADE
        )
    )";

    return query.exec(sql);
}

bool Database::createNotesTable(QSqlQuery& query)
{
    const QString sql = R"(
        CREATE TABLE IF NOT EXISTS notes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            project_id INTEGER NOT NULL,
            title TEXT,
            content TEXT NOT NULL,
            created_at DATETIME NOT NULL,
            updated_at DATETIME NOT NULL,
            FOREIGN KEY (project_id) REFERENCES projects (id) ON DELETE CASCADE
        )
    )";

    return query.exec(sql);
}

bool Database::createEditorsTable(QSqlQuery& query)
{
    const QString sql = R"(
        CREATE TABLE IF NOT EXISTS editors (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            path TEXT NOT NULL,
            arguments TEXT,
            enabled BOOLEAN NOT NULL DEFAULT 1,
            display_order INTEGER NOT NULL DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(name)
        )
    )";

    return query.exec(sql);
}

bool Database::createProcessTemplatesTable(QSqlQuery& query)
{
    const QString sql = R"(
        CREATE TABLE IF NOT EXISTS process_templates (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE NOT NULL,
            command TEXT NOT NULL,
            port INTEGER,
            description TEXT,
            parameters TEXT,
            environment TEXT,
            enabled BOOLEAN NOT NULL DEFAULT 1,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    return query.exec(sql);
}

bool Database::createAppsTables(QSqlQuery& query)
{
    const QString appsTable = R"(
        CREATE TABLE IF NOT EXISTS apps (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            path TEXT NOT NULL,
            arguments TEXT,
            enabled BOOLEAN NOT NULL DEFAULT 1,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(name)
        )
    )";

    const QString projectAppsTable = R"(
        CREATE TABLE IF NOT EXISTS project_apps (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            project_id INTEGER NOT NULL,
            app_id INTEGER NOT NULL,
            FOREIGN KEY (project_id) REFERENCES projects (id) ON DELETE CASCADE,
            FOREIGN KEY (app_id) REFERENCES apps (id) ON DELETE CASCADE,
            UNIQUE(project_id, app_id)
        )
    )";

    return query.exec(appsTable) && query.exec(projectAppsTable);
}

bool Database::createSnippetTable(QSqlQuery& query)
{
    const QString sql = R"(
        CREATE TABLE IF NOT EXISTS snippets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            language TEXT NOT NULL,
            code TEXT NOT NULL,
            description TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    return query.exec(sql);
}

bool Database::verifyDatabase()
{
    QSqlQuery query(db);
    return query.exec("PRAGMA quick_check");
}

bool Database::execute(const QString& query, const QVariantMap& params)
{
    QSqlQuery q(db);

    q.prepare(query);
    for (auto it = params.constBegin(); it != params.constEnd(); ++it)
    {
        q.bindValue(":" + it.key(), it.value());
    }

    if (!q.exec())
    {
        qWarning() << "Query failed:" << query << "\nError:" << q.lastError();
        return false;
    }
    return true;
}
