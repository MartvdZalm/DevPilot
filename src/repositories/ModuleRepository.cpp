#include "ModuleRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

ModuleRepository::ModuleRepository(QSqlDatabase& db) : database(db) {}

std::optional<Module> ModuleRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM modules WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding module ID " + QString::number(id) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto module = mapFromRecord(query);
        LOG_INFO("Successfully found module ID: " + QString::number(id));
        return module;
    }

    LOG_INFO("Module not found with ID: " + QString::number(id));
    return std::nullopt;
}

QList<Module> ModuleRepository::findByProjectId(int projectId)
{
    QList<Module> results;
    QSqlQuery query(database);
    query.prepare("SELECT * FROM modules WHERE project_id = :project_id");
    query.bindValue(":project_id", projectId);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding modules by project ID " + QString::number(projectId) + " : " +
                  query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Found " + QString::number(results.size()) + " modules for project ID: " + QString::number(projectId));
    return results;
}

QList<Module> ModuleRepository::findByStatus(Module::Status status)
{
    QList<Module> results;
    QSqlQuery query(database);
    query.prepare("SELECT * FROM modules WHERE status = :status ORDER BY project_id, name");
    query.bindValue(":status", static_cast<int>(status));

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding components by status " + QString::number(static_cast<int>(status)) +
                  " : " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Found " + QString::number(results.size()) +
             " components with status: " + QString::number(static_cast<int>(status)));
    return results;
}

std::optional<Module> ModuleRepository::findByProjectIdAndPort(int projectId, int port)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM modules WHERE project_id = :project_id AND port = :port");
    query.bindValue(":project_id", projectId);
    query.bindValue(":port", port);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding component by project ID " + QString::number(projectId) + " and port " +
                  QString::number(port) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto component = mapFromRecord(query);
        LOG_INFO("Successfully found component with project ID: " + QString::number(projectId) +
                 " and port: " + QString::number(port));
        return component;
    }

    LOG_INFO("Component not found with project ID: " + QString::number(projectId) +
             " and port: " + QString::number(port));
    return std::nullopt;
}

std::optional<Module> ModuleRepository::save(const Module& component)
{
    return component.getId() > 0 ? update(component) : insert(component);
}

QList<Module> ModuleRepository::findAll()
{
    QList<Module> results;
    QSqlQuery query("SELECT * FROM modules", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all components: " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(results.size()) + " components from database");
    return results;
}

bool ModuleRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM modules WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete component ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted component ID: " + QString::number(id));
    return true;
}

Module ModuleRepository::mapFromRecord(const QSqlQuery& query)
{
    Module component;
    component.setId(query.value("id").toInt());
    component.setProjectId(query.value("project_id").toInt());
    component.setName(query.value("name").toString());
    component.setPort(query.value("port").toInt());
    component.setStatus(static_cast<Module::Status>(query.value("status").toInt()));
    component.setCommand(query.value("command").toString());
    component.setWorkingDirectory(query.value("working_directory").toString());
    component.setLogs(query.value("logs").toString());
    component.setDescription(query.value("description").toString());
    component.setParameters(query.value("parameters").toString());
    component.setEnvironment(query.value("environment").toString());
    component.setServiceType(static_cast<Module::ServiceType>(query.value("service_type").toInt()));
    component.setCreatedAt(query.value("created_at").toDateTime());
    component.setUpdatedAt(query.value("updated_at").toDateTime());
    return component;
}

std::optional<Module> ModuleRepository::insert(const Module& module)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO modules (project_id, name, port, status, command, working_directory, logs, "
                  "description, parameters, environment, service_type, created_at, updated_at) VALUES (:project_id, "
                  ":name, :port, :status, :command, :working_directory, :logs, :description, :parameters, "
                  ":environment, :service_type, :created_at, :updated_at)");

    query.bindValue(":project_id", module.getProjectId());
    query.bindValue(":name", module.getName());
    query.bindValue(":port", module.getPort());
    query.bindValue(":status", static_cast<int>(module.getStatus()));
    query.bindValue(":command", module.getCommand());
    query.bindValue(":working_directory", module.getWorkingDirectory());
    query.bindValue(":logs", module.getLogs());
    query.bindValue(":description", module.getDescription());
    query.bindValue(":parameters", module.getParameters());
    query.bindValue(":environment", module.getEnvironment());
    query.bindValue(":service_type", static_cast<int>(module.getServiceType()));
    query.bindValue(":created_at", QDateTime::currentDateTime());
    query.bindValue(":updated_at", QDateTime::currentDateTime());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert module '" + module.getName() + "' : " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new component ID: " + QString::number(id));
    return findById(id);
}

std::optional<Module> ModuleRepository::update(const Module& component)
{
    QSqlQuery query(database);
    query.prepare("UPDATE modules SET project_id = :project_id, name = :name, port = :port, status = "
                  ":status, command = :command, working_directory = :working_directory, logs = :logs, description = "
                  ":description, parameters = :parameters, environment = :environment, service_type = :service_type, updated_at = :updated_at WHERE id = :id");
    query.bindValue(":project_id", component.getProjectId());
    query.bindValue(":name", component.getName());
    query.bindValue(":port", component.getPort());
    query.bindValue(":status", static_cast<int>(component.getStatus()));
    query.bindValue(":command", component.getCommand());
    query.bindValue(":working_directory", component.getWorkingDirectory());
    query.bindValue(":logs", component.getLogs());
    query.bindValue(":description", component.getDescription());
    query.bindValue(":parameters", component.getParameters());
    query.bindValue(":environment", component.getEnvironment());
    query.bindValue(":service_type", static_cast<int>(component.getServiceType()));
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    query.bindValue(":id", component.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update component ID " + QString::number(component.getId()) + " : " +
                  query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated component ID: " + QString::number(component.getId()));
    return component;
}
