#ifndef MODULETEMPLATESEEDER_H
#define MODULETEMPLATESEEDER_H

#include "../models/ModuleTemplate.h"
#include "../repositories/ModuleTemplateRepository.h"
#include "ISeeder.h"
#include <QVector>

class ModuleTemplateSeeder : public ISeeder
{
  public:
    ModuleTemplateSeeder(ModuleTemplateRepository& repository);

    bool shouldSeed() override;
    bool seed() override;
    QString getName() override;

  private:
    ModuleTemplateRepository& moduleTemplateRepository;
    QVector<ModuleTemplate> getDefaultTemplates();
};

#endif // MODULETEMPLATESEEDER_H