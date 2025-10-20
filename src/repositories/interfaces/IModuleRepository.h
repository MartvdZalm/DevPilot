#ifndef IMODULEREPOSITORY_H
#define IMODULEREPOSITORY_H

#include "../../models/Module.h"
#include <QSqlQuery>
#include <QString>
#include <optional>

class IModuleRepository
{
  public:
    virtual ~IModuleRepository() = default;

    virtual std::optional<Module> findById(int id) = 0;
    virtual QList<Module> findAll() = 0;
    virtual std::optional<Module> save(const Module& module) = 0;
    virtual bool deleteById(int id) = 0;

    virtual QList<Module> findByProjectId(int projectId) = 0;
    virtual QList<Module> findByStatus(Module::Status status) = 0;
    virtual std::optional<Module> findByProjectIdAndPort(int projectId, int port) = 0;
    virtual Module mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // IMODULEREPOSITORY_H
