#ifndef IMODULETEMPLATEREPOSITORY_H
#define IMODULETEMPLATEREPOSITORY_H

#include "../../models/ModuleTemplate.h"
#include <QSqlQuery>
#include <optional>

class IModuleTemplateRepository
{
  public:
    virtual ~IModuleTemplateRepository() = default;
    virtual std::optional<ModuleTemplate> findById(int id) = 0;
    virtual QList<ModuleTemplate> findAll() = 0;
    virtual std::optional<ModuleTemplate> save(const ModuleTemplate& moduleTemplate) = 0;
    virtual bool deleteById(int id) = 0;
    virtual ModuleTemplate mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // MODULETEMPLATE_H
