#include "ModuleTemplateRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

ModuleTemplateRepository::ModuleTemplateRepository(QSqlDatabase& db) : database(db) {}

std::optional<ModuleTemplate> ModuleTemplateRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM module_templates WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding module template ID " + QString::number(id) + " : " +
                  query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto moduleTemplate = mapFromRecord(query);
        LOG_INFO("Successfully found module template ID: " + QString::number(id));
        return moduleTemplate;
    }

    LOG_INFO("Module template not found with ID: " + QString::number(id));
    return std::nullopt;
}

QList<ModuleTemplate> ModuleTemplateRepository::findAll()
{
    QList<ModuleTemplate> templates;
    QSqlQuery query("SELECT * FROM module_templates", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all module templates: " + query.lastError().text());
        return templates;
    }

    while (query.next())
    {
        templates.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(templates.size()) + " module templates from database");
    return templates;
}

std::optional<ModuleTemplate> ModuleTemplateRepository::save(const ModuleTemplate& moduleTemplate)
{
    return moduleTemplate.getId() > 0 ? update(moduleTemplate) : insert(moduleTemplate);
}

bool ModuleTemplateRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM module_templates WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete module template ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted module template ID: " + QString::number(id));
    return true;
}

ModuleTemplate ModuleTemplateRepository::mapFromRecord(const QSqlQuery& query)
{
    ModuleTemplate moduleTemplate;
    moduleTemplate.setId(query.value("id").toInt());
    moduleTemplate.setName(query.value("name").toString());
    moduleTemplate.setCommand(query.value("command").toString());
    moduleTemplate.setPort(query.value("port").toInt());
    moduleTemplate.setDescription(query.value("description").toString());
    moduleTemplate.setParameters(query.value("parameters").toString());
    moduleTemplate.setEnvironment(query.value("environment").toString());
    moduleTemplate.setEnabled(query.value("enabled").toBool());
    moduleTemplate.setCreatedAt(query.value("created_at").toDateTime());
    moduleTemplate.setUpdatedAt(query.value("updated_at").toDateTime());
    return moduleTemplate;
}

std::optional<ModuleTemplate> ModuleTemplateRepository::insert(const ModuleTemplate& moduleTemplate)
{
    QSqlQuery query(database);
    query.prepare(R"(
		INSERT INTO module_templates (name, command, port, description, parameters, environment, enabled, created_at, updated_at)
		VALUES (:name, :command, :port, :description, :parameters, :environment, :enabled, :created_at, :updated_at)
	)");
    query.bindValue(":name", moduleTemplate.getName());
    query.bindValue(":command", moduleTemplate.getCommand());
    query.bindValue(":port", moduleTemplate.getPort());
    query.bindValue(":description", moduleTemplate.getDescription());
    query.bindValue(":parameters", moduleTemplate.getParameters());
    query.bindValue(":environment", moduleTemplate.getEnvironment());
    query.bindValue(":enabled", moduleTemplate.isEnabled());
    query.bindValue(":created_at", QDateTime::currentDateTime());
    query.bindValue(":updated_at", QDateTime::currentDateTime());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert module template '" + moduleTemplate.getName() + "' : " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new module template ID: " + QString::number(id));
    return findById(id);
}

std::optional<ModuleTemplate> ModuleTemplateRepository::update(const ModuleTemplate& moduleTemplate)
{
    QSqlQuery query(database);
    query.prepare(R"(
		UPDATE module_templates
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
    query.bindValue(":name", moduleTemplate.getName());
    query.bindValue(":command", moduleTemplate.getCommand());
    query.bindValue(":port", moduleTemplate.getPort());
    query.bindValue(":description", moduleTemplate.getDescription());
    query.bindValue(":parameters", moduleTemplate.getParameters());
    query.bindValue(":environment", moduleTemplate.getEnvironment());
    query.bindValue(":enabled", moduleTemplate.isEnabled());
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    query.bindValue(":id", moduleTemplate.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update module template ID " + QString::number(moduleTemplate.getId()) + " : " +
                  query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated module template ID: " + QString::number(moduleTemplate.getId()));
    return moduleTemplate;
}
