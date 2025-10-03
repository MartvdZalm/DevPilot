#ifndef MODULETEMPLATEREPOSITORY_H
#define MODULETEMPLATEREPOSITORY_H

#include "IModuleTemplateRepository.h"
#include <QSqlDatabase>

class ModuleTemplateRepository : public IModuleTemplateRepository
{
  public:
    explicit ModuleTemplateRepository(QSqlDatabase& db);

    std::optional<ModuleTemplate> findById(int id) override;
    QList<ModuleTemplate> findAll() override;
    std::optional<ModuleTemplate> save(const ModuleTemplate& moduleTemplate) override;
    bool deleteById(int id) override;
    ModuleTemplate mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<ModuleTemplate> insert(const ModuleTemplate& moduleTemplate);
    std::optional<ModuleTemplate> update(const ModuleTemplate& moduleTemplate);
};

#endif // MODULETEMPLATEREPOSITORY_H