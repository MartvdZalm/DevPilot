#ifndef NOTEREPOSITORY_H
#define NOTEREPOSITORY_H

#include "interfaces/INoteRepository.h"
#include <QSqlDatabase>

class NoteRepository : public INoteRepository
{
  public:
    explicit NoteRepository(QSqlDatabase& db);

    std::optional<Note> findById(int id) override;
    QList<Note> findAll() override;
    std::optional<Note> save(const Note& note) override;
    bool deleteById(int id) override;
    QList<Note> findByProjectId(int projectId) override;
    Note mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<Note> insert(const Note& note);
    std::optional<Note> update(const Note& note);
};

#endif // NOTEREPOSITORY_H
