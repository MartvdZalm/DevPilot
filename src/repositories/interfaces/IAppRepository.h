#ifndef IAPPREPOSITORY_H
#define IAPPREPOSITORY_H

#include "../../models/App.h"
#include <QSqlQuery>
#include <optional>

class IAppRepository
{
  public:
    virtual ~IAppRepository() = default;

    virtual std::optional<App> findById(int id) = 0;
    virtual QList<App> findAll() = 0;
    virtual std::optional<App> save(const App& app) = 0;
    virtual bool deleteById(int id) = 0;
    virtual QList<App> findByProjectId(int projectId) = 0;
    virtual bool setLinkedApps(int projectId, const QList<int>& appIds) = 0;

    virtual App mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // IAPPREPOSITORY_H
