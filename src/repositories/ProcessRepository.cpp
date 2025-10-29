#include "ProcessRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

ProcessRepository::ProcessRepository(QSqlDatabase& db) : database(db) {}

std::optional<Process> ProcessRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM processes WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding module ID " + QString::number(id) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto module = mapFromRecord(query);
        LOG_INFO("Successfully found process ID: " + QString::number(id));
        return module;
    }

    LOG_INFO("Process not found with ID: " + QString::number(id));
    return std::nullopt;
}

QList<Process> ProcessRepository::findByProjectId(int projectId)
{
    QList<Process> results;
    QSqlQuery query(database);
    query.prepare("SELECT * FROM processes WHERE project_id = :project_id");
    query.bindValue(":project_id", projectId);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding processes by project ID " + QString::number(projectId) + " : " +
                  query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Found " + QString::number(results.size()) + " processes for project ID: " + QString::number(projectId));
    return results;
}

std::optional<Process> ProcessRepository::save(const Process& process)
{
    return process.getId() > 0 ? update(process) : insert(process);
}

QList<Process> ProcessRepository::findAll()
{
    QList<Process> results;
    QSqlQuery query("SELECT * FROM processes", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all processes: " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(results.size()) + " processes from database");
    return results;
}

bool ProcessRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM processes WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete process ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted process ID: " + QString::number(id));
    return true;
}

Process ProcessRepository::mapFromRecord(const QSqlQuery& query)
{
    Process process;
    process.setId(query.value("id").toInt());
    process.setProjectId(query.value("project_id").toInt());
    process.setName(query.value("name").toString());
    process.setCommand(query.value("command").toString());
    process.setWorkingDirectory(query.value("working_directory").toString());
    process.setStatus(Process::fromString(query.value("status").toString()));
    process.setPID(query.value("pid").toInt());
    process.setPort(query.value("port").toInt());
    process.setLogPath(query.value("log_path").toString());
    process.setLastStartedAt(query.value("last_started_at").toDateTime());
    process.setUptime(query.value("uptime").toDateTime());
    process.setCreatedAt(query.value("created_at").toDateTime());
    process.setUpdatedAt(query.value("updated_at").toDateTime());
    return process;
}

std::optional<Process> ProcessRepository::insert(const Process& process)
{
    QSqlQuery query(database);

    query.prepare(R"(
        INSERT INTO processes (
            project_id,
            name,
            command,
            working_directory,
            status,
            pid,
            port,
            log_path,
            last_started_at,
            uptime
        ) VALUES (
            :project_id,
            :name,
            :command,
            :working_directory,
            :status,
            :pid,
            :port,
            :log_path,
            :last_started_at,
            :uptime
        )
    )");

    query.bindValue(":project_id", process.getProjectId());
    query.bindValue(":name", process.getName());
    query.bindValue(":command", process.getCommand());
    query.bindValue(":working_directory", process.getWorkingDirectory());
    query.bindValue(":status", process.getStatusString());
    query.bindValue(":pid", process.getPID());
    query.bindValue(":port", process.getPort());
    query.bindValue(":log_path", process.getLogPath());
    query.bindValue(":last_started_at",
                    process.getLastStartedAt().isValid() ? process.getLastStartedAt() : QVariant());
    query.bindValue(":uptime", process.getUptime().isValid() ? process.getUptime() : QVariant());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert process '" + process.getName() + "' : " + query.lastError().text());
        return std::nullopt;
    }

    int id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new process ID: " + QString::number(id));
    return findById(id);
}

std::optional<Process> ProcessRepository::update(const Process& process)
{
    QSqlQuery query(database);
    query.prepare(R"(
        UPDATE processes
        SET
            project_id = :project_id,
            name = :name,
            command = :command,
            working_directory = :working_directory,
            status = :status,
            pid = :pid,
            port = :port,
            log_path = :log_path,
            last_started_at = :last_started_at,
            uptime = :uptime,
            updated_at = :updated_at
        WHERE id = :id
    )");

    query.bindValue(":project_id", process.getProjectId());
    query.bindValue(":name", process.getName());
    query.bindValue(":command", process.getCommand());
    query.bindValue(":working_directory", process.getWorkingDirectory());
    query.bindValue(":status", process.getStatusString());
    query.bindValue(":pid", process.getPID());
    query.bindValue(":port", process.getPort());
    query.bindValue(":log_path", process.getLogPath());
    query.bindValue(":last_started_at",
                    process.getLastStartedAt().isValid() ? process.getLastStartedAt() : QVariant());
    query.bindValue(":uptime", process.getUptime().isValid() ? process.getUptime() : QVariant());
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    query.bindValue(":id", process.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update process ID " + QString::number(process.getId()) + " : " + query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated process ID: " + QString::number(process.getId()));
    return findById(process.getId());
}
