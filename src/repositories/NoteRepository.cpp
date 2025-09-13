#include "NoteRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

NoteRepository::NoteRepository(QSqlDatabase& db) : database(db) {}

std::optional<Note> NoteRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM notes WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding note ID " + QString::number(id) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto note = mapFromRecord(query);
        LOG_INFO("Successfully found note ID: " + QString::number(id));
        return note;
    }

    LOG_INFO("Note not found with ID: " + QString::number(id));
    return std::nullopt;
}

QList<Note> NoteRepository::findByProjectId(int projectId)
{
    QList<Note> results;
    QSqlQuery query(database);
    query.prepare("SELECT * FROM notes WHERE project_id = :project_id");
    query.bindValue(":project_id", projectId);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding notes by project ID " + QString::number(projectId) + " : " +
                  query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Found " + QString::number(results.size()) + " notes for project ID: " + QString::number(projectId));
    return results;
}

std::optional<Note> NoteRepository::save(const Note& note)
{
    return note.getId() > 0 ? update(note) : insert(note);
}

QList<Note> NoteRepository::findAll()
{
    QList<Note> results;
    QSqlQuery query("SELECT * FROM notes", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all notes: " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(results.size()) + " notes from database");
    return results;
}

bool NoteRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM notes WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete note ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted note ID: " + QString::number(id));
    return true;
}

Note NoteRepository::mapFromRecord(const QSqlQuery& query)
{
    Note note;
    note.setId(query.value("id").toInt());
    note.setProjectId(query.value("project_id").toInt());
    note.setTitle(query.value("title").toString());
    note.setContent(query.value("content").toString());
    note.setCreatedAt(query.value("created_at").toDateTime());
    note.setUpdatedAt(query.value("updated_at").toDateTime());
    return note;
}

std::optional<Note> NoteRepository::insert(const Note& note)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO notes (project_id, title, content,  created_at, updated_at)"
                  "VALUES (:project_id, :title, :content, :created_at, :updated_at)");

    query.bindValue(":project_id", note.getProjectId());
    query.bindValue(":title", note.getTitle());
    query.bindValue(":content", note.getContent());
    query.bindValue(":created_at", QDateTime::currentDateTime());
    query.bindValue(":updated_at", QDateTime::currentDateTime());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert note '" + note.getTitle() + "' : " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new note ID: " + QString::number(id));
    return findById(id);
}

std::optional<Note> NoteRepository::update(const Note& note)
{
    QSqlQuery query(database);
    query.prepare("UPDATE notes SET project_id = :project_id, title = :title, content = :content, updated_at = "
                  ":updated_at WHERE id = :id");
    query.bindValue(":project_id", note.getProjectId());
    query.bindValue(":title", note.getTitle());
    query.bindValue(":content", note.getContent());
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    query.bindValue(":id", note.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update note ID " + QString::number(note.getId()) + " : " + query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated note ID: " + QString::number(note.getId()));
    return note;
}
