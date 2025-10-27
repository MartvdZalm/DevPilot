#ifndef SNIPPETREPOSITORY_H
#define SNIPPETREPOSITORY_H

#include "interfaces/ISnippetRepository.h"
#include <QSqlDatabase>

class SnippetRepository : public ISnippetRepository
{
  public:
    explicit SnippetRepository(QSqlDatabase& db);

    std::optional<Snippet> findById(int id) override;
    QList<Snippet> findAll() override;
    std::optional<Snippet> save(const Snippet& snippet) override;
    bool deleteById(int id) override;

    Snippet mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<Snippet> insert(const Snippet& snippet);
    std::optional<Snippet> update(const Snippet& snippet);
};

#endif // SNIPPETREPOSITORY_H
