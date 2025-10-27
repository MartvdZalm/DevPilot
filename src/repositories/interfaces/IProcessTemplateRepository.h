#ifndef IPROCESSTEMPLATEREPOSITORY_H
#define IPROCESSTEMPLATEREPOSITORY_H

#include "../../models/ProcessTemplate.h"
#include <QSqlQuery>
#include <optional>

class IProcessTemplateRepository
{
  public:
    virtual ~IProcessTemplateRepository() = default;
    virtual std::optional<ProcessTemplate> findById(int id) = 0;
    virtual QList<ProcessTemplate> findAll() = 0;
    virtual std::optional<ProcessTemplate> save(const ProcessTemplate& processTemplate) = 0;
    virtual bool deleteById(int id) = 0;
    virtual ProcessTemplate mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // IPROCESSTEMPLATEREPOSITORY_H
