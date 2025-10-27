#ifndef PROCESSTEMPLATEREPOSITORY_H
#define PROCESSTEMPLATEREPOSITORY_H

#include "interfaces/IProcessTemplateRepository.h"
#include <QSqlDatabase>

class ProcessTemplateRepository : public IProcessTemplateRepository
{
  public:
    explicit ProcessTemplateRepository(QSqlDatabase& db);

    std::optional<ProcessTemplate> findById(int id) override;
    QList<ProcessTemplate> findAll() override;
    std::optional<ProcessTemplate> save(const ProcessTemplate& processTemplate) override;
    bool deleteById(int id) override;
    ProcessTemplate mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<ProcessTemplate> insert(const ProcessTemplate& processTemplate);
    std::optional<ProcessTemplate> update(const ProcessTemplate& processTemplate);
};

#endif // PROCESSTEMPLATEREPOSITORY_H
