#ifndef REPOSITORYPROVIDER_H
#define REPOSITORYPROVIDER_H

#include "interfaces/IModuleRepository.h"
#include "interfaces/INoteRepository.h"
#include "interfaces/IProjectRepository.h"
#include "interfaces/IEditorRepository.h"
#include "interfaces/IModuleTemplateRepository.h"

class RepositoryProvider
{
  public:
    RepositoryProvider(std::unique_ptr<IProjectRepository> projectRepository,
                       std::unique_ptr<INoteRepository> noteRepository,
                       std::unique_ptr<IModuleRepository> moduleRepository,
                       std::unique_ptr<IEditorRepository> editorRepository,
                       std::unique_ptr<IModuleTemplateRepository> moduleTemplateRepository
    )
        : projectRepository(std::move(projectRepository)), noteRepository(std::move(noteRepository)),
          moduleRepository(std::move(moduleRepository)),
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
    std::unique_ptr<IEditorRepository> editorRepository;
    std::unique_ptr<IModuleTemplateRepository> moduleTemplateRepository;
};

#endif // REPOSITORYPROVIDER_H
