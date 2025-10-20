#include "AppRepository.h"
#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

AppRepository::AppRepository(QSqlDatabase& db) : database(db) {}

std::optional<App> AppRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM apps WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding app ID " + QString::number(id) + ": " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        return mapFromRecord(query);
    }

    return std::nullopt;
}

QList<App> AppRepository::findAll()
{
    QList<App> apps;
    QSqlQuery query("SELECT * FROM apps", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all apps: " + query.lastError().text());
        return apps;
    }

    while (query.next())
    {
        apps.append(mapFromRecord(query));
    }

    return apps;
}

std::optional<App> AppRepository::save(const App& app)
{
    return app.getId() > 0 ? update(app) : insert(app);
}

std::optional<App> AppRepository::insert(const App& app)
{
    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO apps (name, path, arguments, enabled, created_at, updated_at)
        VALUES (:name, :path, :arguments, :enabled, :created_at, :updated_at)
    )");

    query.bindValue(":name", app.getName());
    query.bindValue(":path", app.getPath());
    query.bindValue(":arguments", app.getArguments());
    query.bindValue(":enabled", app.isEnabled());
    query.bindValue(":created_at", QDateTime::currentDateTime());
    query.bindValue(":updated_at", QDateTime::currentDateTime());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert app '" + app.getName() + "': " + query.lastError().text());
        return std::nullopt;
    }

    int id = query.lastInsertId().toInt();
    return findById(id);
}

std::optional<App> AppRepository::update(const App& app)
{
    QSqlQuery query(database);
    query.prepare(R"(
        UPDATE apps SET name = :name, path = :path, arguments = :arguments, enabled = :enabled, updated_at = :updated_at
        WHERE id = :id
    )");

    query.bindValue(":name", app.getName());
    query.bindValue(":path", app.getPath());
    query.bindValue(":arguments", app.getArguments());
    query.bindValue(":enabled", app.isEnabled());
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    query.bindValue(":id", app.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update app ID " + QString::number(app.getId()) + ": " + query.lastError().text());
        return std::nullopt;
    }

    return app;
}

bool AppRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM apps WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete app ID " + QString::number(id) + ": " + query.lastError().text());
        return false;
    }

    return true;
}

QList<App> AppRepository::findByProjectId(int projectId)
{
    QList<App> apps;

    QSqlQuery query(database);
    query.prepare(R"(
        SELECT a.* FROM apps a
        INNER JOIN project_apps pa ON a.id = pa.app_id
        WHERE pa.project_id = ?
    )");
    query.addBindValue(projectId);

    if (!query.exec())
    {
        LOG_ERROR("Failed to fetch apps for project ID " + QString::number(projectId) + ": " + query.lastError().text());
        return apps;
    }

    while (query.next())
    {
        apps.append(mapFromRecord(query));
    }

    return apps;
}

bool AppRepository::setLinkedApps(int projectId, const QList<int>& appIds)
{
    QSqlQuery deleteQuery(database);
    deleteQuery.prepare("DELETE FROM project_apps WHERE project_id = ?");
    deleteQuery.addBindValue(projectId);
    if (!deleteQuery.exec()) return false;

    for (int appId : appIds)
    {
        QSqlQuery insertQuery(database);
        insertQuery.prepare("INSERT INTO project_apps (project_id, app_id) VALUES (?, ?)");
        insertQuery.addBindValue(projectId);
        insertQuery.addBindValue(appId);
        if (!insertQuery.exec()) return false;
    }

    return true;
}

App AppRepository::mapFromRecord(const QSqlQuery& query)
{
    App app;
    app.setId(query.value("id").toInt());
    app.setName(query.value("name").toString());
    app.setPath(query.value("path").toString());
    app.setArguments(query.value("arguments").toString());
    app.setEnabled(query.value("enabled").toBool());
    app.setCreatedAt(query.value("created_at").toDateTime());
    app.setUpdatedAt(query.value("updated_at").toDateTime());
    return app;
}
