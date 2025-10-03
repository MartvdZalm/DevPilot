#include "EditorRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

EditorRepository::EditorRepository(QSqlDatabase& db) : database(db) {}

std::optional<Editor> EditorRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM editors WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding editor ID " + QString::number(id) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto editor = mapFromRecord(query);
        LOG_INFO("Successfully found editor ID: " + QString::number(id));
        return editor;
    }

    LOG_INFO("Editor not found with ID: " + QString::number(id));
    return std::nullopt;
}

std::optional<Editor> EditorRepository::save(const Editor& editor)
{
    return editor.getId() > 0 ? update(editor) : insert(editor);
}

QList<Editor> EditorRepository::findAll()
{
    QList<Editor> results;
    QSqlQuery query("SELECT * FROM editors", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all editors: " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(results.size()) + " editors from database");
    return results;
}

bool EditorRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM editors WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete editor ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted editor ID: " + QString::number(id));
    return true;
}

Editor EditorRepository::mapFromRecord(const QSqlQuery& query)
{
    Editor editor;
    editor.setId(query.value("id").toInt());
    editor.setName(query.value("name").toString());
    editor.setPath(query.value("path").toString());
    editor.setArguments(query.value("arguments").toString());
    editor.setEnabled(query.value("enabled").toBool());
    editor.setDisplayOrder(query.value("display_order").toInt());
    editor.setCreatedAt(query.value("created_at").toDateTime());
    editor.setUpdatedAt(query.value("updated_at").toDateTime());
    return editor;
}

std::optional<Editor> EditorRepository::insert(const Editor& editor)
{
    QSqlQuery query(database);

    query.prepare("INSERT INTO editors (name, path, arguments, enabled, display_order, created_at, updated_at)"
                  "VALUES (:name, :path, :arguments, :enabled, :display_order, :created_at, :updated_at)");

    query.bindValue(":name", editor.getName());
    query.bindValue(":path", editor.getPath());
    query.bindValue(":arguments", editor.getArguments());
    query.bindValue(":enabled", editor.isEnabled());
    query.bindValue(":display_order", editor.getDisplayOrder());
    query.bindValue(":created_at", QDateTime::currentDateTime());
    query.bindValue(":updated_at", QDateTime::currentDateTime());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert editor '" + editor.getName() + "' : " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new editor ID: " + QString::number(id));
    return findById(id);
}

std::optional<Editor> EditorRepository::update(const Editor& editor)
{
    QSqlQuery query(database);
    query.prepare("UPDATE editors SET name = :name, path = :path, arguments = :arguments, enabled = :enabled, "
                  "display_order = :display_order, updated_at = :updated_at WHERE id = :id");
    query.bindValue(":name", editor.getName());
    query.bindValue(":path", editor.getPath());
    query.bindValue(":arguments", editor.getArguments());
    query.bindValue(":enabled", editor.isEnabled());
    query.bindValue(":display_order", editor.getDisplayOrder());
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    query.bindValue(":id", editor.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update editor ID " + QString::number(editor.getId()) + " : " + query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated editor ID: " + QString::number(editor.getId()));
    return editor;
}
