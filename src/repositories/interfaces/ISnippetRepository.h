#ifndef ISNIPPETREPOSITORY_H
#define ISNIPPETREPOSITORY_H

#include "../../models/Snippet.h"
#include <QSqlQuery>
#include <QString>

class ISnippetRepository
{
  public:
    virtual ~ISnippetRepository() = default;

    virtual std::optional<Snippet> findById(int id) = 0;
    virtual QList<Snippet> findAll() = 0;
    virtual std::optional<Snippet> save(const Snippet& snippet) = 0;
    virtual bool deleteById(int id) = 0;

    virtual Snippet mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // ISNIPPETREPOSITORY_H
