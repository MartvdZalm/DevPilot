#ifndef MODULEREPOSITORY_H
#define MODULEREPOSITORY_H

#include "IModuleRepository.h"
#include <QSqlDatabase>

class ModuleRepository : public IModuleRepository
{
  public:
    explicit ModuleRepository(QSqlDatabase& db);

    std::optional<Module> findById(int id) override;
    QList<Module> findAll() override;
    std::optional<Module> save(const Module& component) override;
    bool deleteById(int id) override;
    QList<Module> findByProjectId(int projectId) override;
    QList<Module> findByStatus(Module::Status status) override;
    std::optional<Module> findByProjectIdAndPort(int projectId, int port) override;
    Module mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<Module> insert(const Module& component);
    std::optional<Module> update(const Module& component);
};

#endif // MODULEREPOSITORY_H
