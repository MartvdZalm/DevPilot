#include "ProcessTemplateRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

ProcessTemplateRepository::ProcessTemplateRepository(QSqlDatabase& db) : database(db) {}

std::optional<ProcessTemplate> ProcessTemplateRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM process_templates WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding process template ID " + QString::number(id) + " : " +
                  query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto processTemplate = mapFromRecord(query);
        LOG_INFO("Successfully found process template ID: " + QString::number(id));
        return processTemplate;
    }

    LOG_INFO("Process template not found with ID: " + QString::number(id));
    return std::nullopt;
}

QList<ProcessTemplate> ProcessTemplateRepository::findAll()
{
    QList<ProcessTemplate> templates;
    QSqlQuery query("SELECT * FROM process_templates", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all process templates: " + query.lastError().text());
        return templates;
    }

    while (query.next())
    {
        templates.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(templates.size()) + " process templates from database");
    return templates;
}

std::optional<ProcessTemplate> ProcessTemplateRepository::save(const ProcessTemplate& processTemplate)
{
    return processTemplate.getId() > 0 ? update(processTemplate) : insert(processTemplate);
}

bool ProcessTemplateRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM process_templates WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete process template ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted process template ID: " + QString::number(id));
    return true;
}

ProcessTemplate ProcessTemplateRepository::mapFromRecord(const QSqlQuery& query)
{
    ProcessTemplate processTemplate;
    processTemplate.setId(query.value("id").toInt());
    processTemplate.setName(query.value("name").toString());
    processTemplate.setCommand(query.value("command").toString());
    processTemplate.setPort(query.value("port").toInt());
    processTemplate.setDescription(query.value("description").toString());
    processTemplate.setParameters(query.value("parameters").toString());
    processTemplate.setEnvironment(query.value("environment").toString());
    processTemplate.setEnabled(query.value("enabled").toBool());
    processTemplate.setCreatedAt(query.value("created_at").toDateTime());
    processTemplate.setUpdatedAt(query.value("updated_at").toDateTime());
    return processTemplate;
}

std::optional<ProcessTemplate> ProcessTemplateRepository::insert(const ProcessTemplate& processTemplate)
{
    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO process_templates (name, command, port, description, parameters, environment, enabled, created_at, updated_at)
		VALUES (:name, :command, :port, :description, :parameters, :environment, :enabled, :created_at, :updated_at)
	)");
    query.bindValue(":name", processTemplate.getName());
    query.bindValue(":command", processTemplate.getCommand());
    query.bindValue(":port", processTemplate.getPort());
    query.bindValue(":description", processTemplate.getDescription());
    query.bindValue(":parameters", processTemplate.getParameters());
    query.bindValue(":environment", processTemplate.getEnvironment());
    query.bindValue(":enabled", processTemplate.isEnabled());
    query.bindValue(":created_at", QDateTime::currentDateTime());
    query.bindValue(":updated_at", QDateTime::currentDateTime());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert process template '" + processTemplate.getName() +
                  "' : " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new process template ID: " + QString::number(id));
    return findById(id);
}

std::optional<ProcessTemplate> ProcessTemplateRepository::update(const ProcessTemplate& processTemplate)
{
    QSqlQuery query(database);
    query.prepare(R"(
        UPDATE process_templates
		SET name = :name,
			command = :command,
			port = :port,
			description = :description,
			parameters = :parameters,
			environment = :environment,
            enabled = :enabled,
			updated_at = :updated_at
		WHERE id = :id
	)");
    query.bindValue(":name", processTemplate.getName());
    query.bindValue(":command", processTemplate.getCommand());
    query.bindValue(":port", processTemplate.getPort());
    query.bindValue(":description", processTemplate.getDescription());
    query.bindValue(":parameters", processTemplate.getParameters());
    query.bindValue(":environment", processTemplate.getEnvironment());
    query.bindValue(":enabled", processTemplate.isEnabled());
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    query.bindValue(":id", processTemplate.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update process template ID " + QString::number(processTemplate.getId()) + " : " +
                  query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated process template ID: " + QString::number(processTemplate.getId()));
    return processTemplate;
}
