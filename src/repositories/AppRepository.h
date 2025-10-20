#ifndef APPREPOSITORY_H
#define APPREPOSITORY_H

#include "interfaces/IAppRepository.h"
#include <QSqlDatabase>

class AppRepository : public IAppRepository
{
  public:
    explicit AppRepository(QSqlDatabase& db);

    std::optional<App> findById(int id) override;
    QList<App> findAll() override;
    std::optional<App> save(const App& app) override;
    bool deleteById(int id) override;
    QList<App> findByProjectId(int projectId) override;
    bool setLinkedApps(int projectId, const QList<int>& appIds) override;

    App mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;

    std::optional<App> insert(const App& app);
    std::optional<App> update(const App& app);
};

#endif // APPREPOSITORY_H
