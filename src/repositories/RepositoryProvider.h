#ifndef REPOSITORYPROVIDER_H
#define REPOSITORYPROVIDER_H

#include "IModuleRepository.h"
#include "INoteRepository.h"
#include "IProjectRepository.h"
#include "ISettingRepository.h"
#include "IEditorRepository.h"
#include "IModuleTemplateRepository.h"

class RepositoryProvider
{
  public:
    RepositoryProvider(std::unique_ptr<IProjectRepository> projectRepository,
                       std::unique_ptr<INoteRepository> noteRepository,
                       std::unique_ptr<IModuleRepository> moduleRepository,
                       std::unique_ptr<ISettingRepository> settingRepository,
                       std::unique_ptr<IEditorRepository> editorRepository,
                       std::unique_ptr<IModuleTemplateRepository> moduleTemplateRepository
    )
        : projectRepository(std::move(projectRepository)), noteRepository(std::move(noteRepository)),
          moduleRepository(std::move(moduleRepository)), settingRepository(std::move(settingRepository)),
          editorRepository(std::move(editorRepository)), moduleTemplateRepository(std::move(moduleTemplateRepository))
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

    IModuleTemplateRepository& getModuleTemplateRepository() const
    {
        return *moduleTemplateRepository;
    }

  private:
    std::unique_ptr<IProjectRepository> projectRepository;
    std::unique_ptr<INoteRepository> noteRepository;
    std::unique_ptr<IModuleRepository> moduleRepository;
    std::unique_ptr<ISettingRepository> settingRepository;
    std::unique_ptr<IEditorRepository> editorRepository;
    std::unique_ptr<IModuleTemplateRepository> moduleTemplateRepository;
};

#endif // REPOSITORYPROVIDER_H
