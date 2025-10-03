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

    if (!applyPragmas())
    {
        qCritical() << "Failed to apply database optimizations";
        return false;
    }

    if (!createTables())
    {
        qCritical() << "Failed to create database tables";
        return false;
    }

    if (!verifyDatabase())
    {
        qCritical() << "Database verification failed";
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

    const QStringList tables = {
        R"(
        CREATE TABLE IF NOT EXISTS projects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            directory_path TEXT NOT NULL,
            description TEXT,
            created_at DATETIME NOT NULL,
            updated_at DATETIME NOT NULL,
            UNIQUE(name, directory_path)
        )
        )",
        R"(
        CREATE TABLE IF NOT EXISTS modules (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            project_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            port INTEGER,
            status INTEGER NOT NULL DEFAULT 0,
            command TEXT,
            working_directory TEXT,
            logs TEXT,
            description TEXT,
            parameters TEXT,
            environment TEXT,
            auto_start BOOLEAN NOT NULL DEFAULT 0,
            created_at DATETIME NOT NULL,
            updated_at DATETIME NOT NULL,
            FOREIGN KEY (project_id) REFERENCES projects (id) ON DELETE CASCADE,
            UNIQUE(project_id, port)
        )
        )",
        R"(
        CREATE TABLE IF NOT EXISTS notes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            project_id INTEGER NOT NULL,
            title TEXT,
            content TEXT NOT NULL,
            created_at DATETIME NOT NULL,
            updated_at DATETIME NOT NULL,
            FOREIGN KEY (project_id) REFERENCES projects (id) ON DELETE CASCADE
        );
        )",
        R"(
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
        )",
        R"(
        CREATE TABLE IF NOT EXISTS module_templates (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE NOT NULL,
            command TEXT NOT NULL,
            port INTEGER NOT NULL,
            description TEXT,
            parameters TEXT,
            environment TEXT,
            enabled BOOLEAN NOT NULL DEFAULT 1,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
        )"};

    for (const auto& table : tables)
    {
        if (!query.exec(table))
        {
            qCritical() << "Table creation failed:" << query.lastError();
            query.exec("ROLLBACK");
            return false;
        }
    }

    if (!query.exec("COMMIT"))
    {
        qCritical() << "Commit failed:" << query.lastError();
        return false;
    }

    return true;
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
