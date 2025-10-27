#ifndef PROCESSTEMPLATESEEDER_H
#define PROCESSTEMPLATESEEDER_H

#include "../../models/ProcessTemplate.h"
#include "../../repositories/ProcessTemplateRepository.h"
#include "ISeeder.h"
#include <QVector>

class ProcessTemplateSeeder : public ISeeder
{
  public:
    ProcessTemplateSeeder(ProcessTemplateRepository& repository);

    bool shouldSeed() override;
    bool seed() override;
    QString getName() override;

  private:
    ProcessTemplateRepository& processTemplateRepository;
    QVector<ProcessTemplate> getDefaultTemplates();
};

#endif // PROCESSTEMPLATESEEDER_H
