#ifndef IPROJECTREPOSITORY_H
#define IPROJECTREPOSITORY_H

#include "../../models/Project.h"
#include <QSqlQuery>
#include <QString>
#include <optional>
#include <vector>

class IProjectRepository
{
  public:
    virtual ~IProjectRepository() = default;

    virtual std::optional<Project> findById(int id) = 0;
    virtual QList<Project> findAll() = 0;
    virtual std::optional<Project> save(const Project& project) = 0;
    virtual bool deleteById(int id) = 0;

    virtual std::optional<Project> findByName(const QString& name) = 0;
    virtual QList<Project> findByDirectoryPath(const QString& directoryPath) = 0;
    virtual Project mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // IPROJECTREPOSITORY_H
