#ifndef REPOSITORYPROVIDER_H
#define REPOSITORYPROVIDER_H

#include "interfaces/IAppRepository.h"
#include "interfaces/IEditorRepository.h"
#include "interfaces/INoteRepository.h"
#include "interfaces/IProcessRepository.h"
#include "interfaces/IProcessTemplateRepository.h"
#include "interfaces/IProjectRepository.h"

class RepositoryProvider
{
  public:
    RepositoryProvider(std::unique_ptr<IProjectRepository> projectRepository,
                       std::unique_ptr<INoteRepository> noteRepository,
                       std::unique_ptr<IProcessRepository> processRepository,
                       std::unique_ptr<IEditorRepository> editorRepository,
                       std::unique_ptr<IProcessTemplateRepository> processTemplateRepository,
                       std::unique_ptr<IAppRepository> appRepository)
        : projectRepository(std::move(projectRepository)), noteRepository(std::move(noteRepository)),
          processRepository(std::move(processRepository)), editorRepository(std::move(editorRepository)),
          processTemplateRepository(std::move(processTemplateRepository)), appRepository(std::move(appRepository))
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

    IProcessRepository& getProcessRepository() const
    {
        return *processRepository;
    }

    IEditorRepository& getEditorRepository() const
    {
        return *editorRepository;
    }

    IProcessTemplateRepository& getProcessTemplateRepository() const
    {
        return *processTemplateRepository;
    }

    IAppRepository& getAppRepository() const
    {
        return *appRepository;
    }

  private:
    std::unique_ptr<IProjectRepository> projectRepository;
    std::unique_ptr<INoteRepository> noteRepository;
    std::unique_ptr<IProcessRepository> processRepository;
    std::unique_ptr<IEditorRepository> editorRepository;
    std::unique_ptr<IProcessTemplateRepository> processTemplateRepository;
    std::unique_ptr<IAppRepository> appRepository;
};

#endif // REPOSITORYPROVIDER_H
