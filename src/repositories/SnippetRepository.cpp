#include "SnippetRepository.h"
#include "../core/Logger.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

SnippetRepository::SnippetRepository(QSqlDatabase& db) : database(db)
{
}

std::optional<Snippet> SnippetRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM snippets WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding snippet ID " + QString::number(id) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto snippet = mapFromRecord(query);
        LOG_INFO("Successfully found snippet ID: " + QString::number(id));
        return snippet;
    }

    LOG_INFO("Snippet not found with ID: " + QString::number(id));
    return std::nullopt;
}


std::optional<Snippet> SnippetRepository::save(const Snippet& snippet)
{
    return snippet.getId() > 0 ? update(snippet) : insert(snippet);
}

QList<Snippet> SnippetRepository::findAll()
{
    QList<Snippet> results;
    QSqlQuery query("SELECT * FROM snippets", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all snippets: " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(results.size()) + " snippets from database");
    return results;
}

bool SnippetRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM snippets WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete snippet ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted snippet ID: " + QString::number(id));
    return true;
}

Snippet SnippetRepository::mapFromRecord(const QSqlQuery& query)
{
    Snippet snippet;
    snippet.setId(query.value("id").toInt());
    snippet.setTitle(query.value("title").toString());
    snippet.setLanguage(query.value("language").toString());
    snippet.setCode(query.value("code").toString());
    snippet.setDescription(query.value("description").toString());
    snippet.setCreatedAt(query.value("created_at").toDateTime());
    snippet.setUpdatedAt(query.value("updated_at").toDateTime());
    return snippet;
}

std::optional<Snippet> SnippetRepository::insert(const Snippet& snippet)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO snippets (title, language, code, description) "
                  "VALUES (:title, :language, :code, :description)");

    query.bindValue(":title", snippet.getTitle());
    query.bindValue(":language", snippet.getLanguage());
    query.bindValue(":code", snippet.getCode());
    query.bindValue(":description", snippet.getDescription());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert snippet '" + snippet.getTitle() + "' : " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new snippet ID: " + QString::number(id));
    return findById(id);
}

std::optional<Snippet> SnippetRepository::update(const Snippet& snippet)
{
    QSqlQuery query(database);
    query.prepare("UPDATE snippets SET title = :title, language = :language, updated_at = :updated_at,"
                  "code = :code, description = :description "
                  "WHERE id = :id");

    query.bindValue(":id", snippet.getId());
    query.bindValue(":title", snippet.getTitle());
    query.bindValue(":language", snippet.getLanguage());
    query.bindValue(":code", snippet.getCode());
    query.bindValue(":description", snippet.getDescription());
    query.bindValue(":updated_at", QDateTime::currentDateTime());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update snippet ID " + QString::number(snippet.getId()) + " : " + query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated snippet ID: " + QString::number(snippet.getId()));
    return snippet;
}
