#ifndef PROJECTREPOSITORY_H
#define PROJECTREPOSITORY_H

#include "IProjectRepository.h"
#include <QSqlDatabase>

class ProjectRepository : public IProjectRepository
{
  public:
    explicit ProjectRepository(QSqlDatabase& db);

    std::optional<Project> findById(int id) override;
    QList<Project> findAll() override;
    std::optional<Project> save(const Project& project) override;
    bool deleteById(int id) override;
    std::optional<Project> findByName(const QString& name) override;
    QList<Project> findByDirectoryPath(const QString& directoryPath) override;
    Project mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<Project> insert(const Project& project);
    std::optional<Project> update(const Project& project);
};

#endif // PROJECTREPOSITORY_H
