#ifndef IPROCESSREPOSITORY_H
#define IPROCESSREPOSITORY_H

#include "../../models/Process.h"
#include <QSqlQuery>
#include <QString>
#include <optional>

class IProcessRepository
{
  public:
    virtual ~IProcessRepository() = default;

    virtual std::optional<Process> findById(int id) = 0;
    virtual QList<Process> findAll() = 0;
    virtual std::optional<Process> save(const Process& process) = 0;
    virtual bool deleteById(int id) = 0;

    virtual QList<Process> findByProjectId(int projectId) = 0;
    virtual Process mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // IPROCESSREPOSITORY_H
