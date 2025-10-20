#ifndef IEDITORREPOSITORY_H
#define IEDITORREPOSITORY_H

#include "../../models/Editor.h"
#include <QSqlQuery>
#include <optional>

class IEditorRepository
{
  public:
    virtual ~IEditorRepository() = default;

    virtual std::optional<Editor> findById(int id) = 0;
    virtual QList<Editor> findAll() = 0;
    virtual std::optional<Editor> save(const Editor& editor) = 0;
    virtual bool deleteById(int id) = 0;

    virtual Editor mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // IEDITORREPOSITORY_H
