#ifndef REPOSITORYPROVIDER_H
#define REPOSITORYPROVIDER_H

#include "IModuleRepository.h"
#include "INoteRepository.h"
#include "IProjectRepository.h"
#include "ISettingRepository.h"

class RepositoryProvider
{
  public:
    RepositoryProvider(
        std::shared_ptr<IProjectRepository> projectRepository,
        std::shared_ptr<INoteRepository> noteRepository,
        std::shared_ptr<IModuleRepository> moduleRepository,
        std::shared_ptr<ISettingRepository> settingRepository)
        : projectRepository(std::move(projectRepository)),
          noteRepository(std::move(noteRepository)),
          moduleRepository(std::move(moduleRepository)),
          settingRepository(std::move(settingRepository)) {}

    std::shared_ptr<IProjectRepository> getProjectRepository() const { return projectRepository; }
    std::shared_ptr<INoteRepository> getNoteRepository() const { return noteRepository; }
    std::shared_ptr<IModuleRepository> getModuleRepository() const { return moduleRepository; }
    std::shared_ptr<ISettingRepository> getSettingRepository() const { return settingRepository; }

  private:
    std::shared_ptr<IProjectRepository> projectRepository;
    std::shared_ptr<INoteRepository> noteRepository;
    std::shared_ptr<IModuleRepository> moduleRepository;
    std::shared_ptr<ISettingRepository> settingRepository;
};

#endif // REPOSITORYPROVIDER_H
