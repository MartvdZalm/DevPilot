#ifndef INOTEREPOSITORY_H
#define INOTEREPOSITORY_H

#include "../models/Note.h"
#include <QSqlQuery>
#include <optional>

class INoteRepository
{
  public:
    virtual ~INoteRepository() = default;

    virtual std::optional<Note> findById(int id) = 0;
    virtual QList<Note> findAll() = 0;
    virtual std::optional<Note> save(const Note& note) = 0;
    virtual bool deleteById(int id) = 0;

    virtual QList<Note> findByProjectId(int projectId) = 0;
    virtual Note mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // INOTEREPOSITORY_H