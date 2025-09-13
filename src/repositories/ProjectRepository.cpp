#include "ProjectRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QString>

ProjectRepository::ProjectRepository(QSqlDatabase& db) : database(db) {}

std::optional<Project> ProjectRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM projects WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding project ID " + QString::number(id) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto project = mapFromRecord(query);
        LOG_INFO("Successfully found project ID: " + QString::number(id));
        return project;
    }

    LOG_INFO("Project not found with ID: " + QString::number(id));
    return std::nullopt;
}

std::optional<Project> ProjectRepository::findByName(const QString& name)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM projects WHERE name = :name");
    query.bindValue(":name", name);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding project with name " + name + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto project = mapFromRecord(query);
        LOG_INFO("Successfully found project with name: " + name);
        return project;
    }

    LOG_INFO("Project not found with name: " + name);
    return std::nullopt;
}

QList<Project> ProjectRepository::findByDirectoryPath(const QString& directoryPath)
{
    QList<Project> results;
    QSqlQuery query(database);
    query.prepare("SELECT * FROM projects WHERE directory_path = :directory_path");
    query.bindValue(":directory_path", directoryPath);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding projects by directory path " + directoryPath + " : " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Found " + QString::number(results.size()) + " projects with directory path: " + directoryPath);
    return results;
}

std::optional<Project> ProjectRepository::save(const Project& project)
{
    return project.getId() > 0 ? update(project) : insert(project);
}

QList<Project> ProjectRepository::findAll()
{
    QList<Project> results;
    QSqlQuery query("SELECT * FROM projects ORDER BY name", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all projects: " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(results.size()) + " projects from database");
    return results;
}

bool ProjectRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM projects WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete project ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted project ID: " + QString::number(id));
    return true;
}

Project ProjectRepository::mapFromRecord(const QSqlQuery& query)
{
    Project project;
    project.setId(query.value("id").toInt());
    project.setName(query.value("name").toString());
    project.setDirectoryPath(query.value("directory_path").toString());
    project.setDescription(query.value("description").toString());
    project.setCreatedAt(query.value("created_at").toDateTime());
    project.setUpdatedAt(query.value("updated_at").toDateTime());
    return project;
}

std::optional<Project> ProjectRepository::insert(const Project& project)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO projects (name, directory_path, description, created_at, updated_at) VALUES (:name, :directory_path, :description, :created_at, :updated_at)");

    query.bindValue(":name", project.getName());
    query.bindValue(":directory_path", project.getDirectoryPath());
    query.bindValue(":description", project.getDescription());
    query.bindValue(":created_at", QDateTime::currentDateTime());
    query.bindValue(":updated_at", QDateTime::currentDateTime());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert project '" + project.getName() + "' : " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new project ID: " + QString::number(id));
    return findById(id);
}

std::optional<Project> ProjectRepository::update(const Project& project)
{
    QSqlQuery query(database);
    query.prepare("UPDATE projects SET name = :name, directory_path = :directory_path, description = :description, updated_at = :updated_at WHERE id = :id");
    query.bindValue(":name", project.getName());
    query.bindValue(":directory_path", project.getDirectoryPath());
    query.bindValue(":description", project.getDescription());
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    query.bindValue(":id", project.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update project ID " + QString::number(project.getId()) + " : " + query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated project ID: " + QString::number(project.getId()));
    return project;
}
