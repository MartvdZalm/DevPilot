#ifndef REPOSITORYPROVIDER_H
#define REPOSITORYPROVIDER_H

#include "IModuleRepository.h"
#include "INoteRepository.h"
#include "IProjectRepository.h"
#include "ISettingRepository.h"
#include "IEditorRepository.h"

class RepositoryProvider
{
  public:
    RepositoryProvider(std::unique_ptr<IProjectRepository> projectRepository,
                       std::unique_ptr<INoteRepository> noteRepository,
                       std::unique_ptr<IModuleRepository> moduleRepository,
                       std::unique_ptr<ISettingRepository> settingRepository,
                       std::unique_ptr<IEditorRepository> editorRepository)
        : projectRepository(std::move(projectRepository)), noteRepository(std::move(noteRepository)),
          moduleRepository(std::move(moduleRepository)), settingRepository(std::move(settingRepository)),
          editorRepository(std::move(editorRepository))
    {
    }

    IProjectRepository& getProjectRepository() const
    {
        return *projectRepository;
    }

    INoteRepository& getNoteRepository() const
    {
        return *noteRepository;
    }

    IModuleRepository& getModuleRepository() const
    {
        return *moduleRepository;
    }

    ISettingRepository& getSettingRepository() const
    {
        return *settingRepository;
    }

    IEditorRepository& getEditorRepository() const
    {
        return *editorRepository;
    }

  private:
    std::unique_ptr<IProjectRepository> projectRepository;
    std::unique_ptr<INoteRepository> noteRepository;
    std::unique_ptr<IModuleRepository> moduleRepository;
    std::unique_ptr<ISettingRepository> settingRepository;
    std::unique_ptr<IEditorRepository> editorRepository;
};

#endif // REPOSITORYPROVIDER_H
