#ifndef PROCESSREPOSITORY_H
#define PROCESSREPOSITORY_H

#include "interfaces/IProcessRepository.h"
#include <QSqlDatabase>

class ProcessRepository : public IProcessRepository
{
  public:
    explicit ProcessRepository(QSqlDatabase& db);

    std::optional<Process> findById(int id) override;
    QList<Process> findAll() override;
    std::optional<Process> save(const Process& process) override;
    bool deleteById(int id) override;
    QList<Process> findByProjectId(int projectId) override;
    Process mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<Process> insert(const Process& process);
    std::optional<Process> update(const Process& process);
};

#endif // PROCESSREPOSITORY_H
