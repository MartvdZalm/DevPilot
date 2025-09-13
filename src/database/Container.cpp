#include "Container.h"

#include "../repositories/ModuleRepository.h"
#include "../repositories/NoteRepository.h"
#include "../repositories/ProjectRepository.h"

Container& Container::instance()
{
    static Container instance;
    return instance;
}

void Container::initialize(QSqlDatabase& database)
{
    this->database = &database;
}

std::shared_ptr<IProjectRepository> Container::getProjectRepository()
{
    if (!projectRepository && database)
    {
        projectRepository = std::make_shared<ProjectRepository>(*database);
    }
    return projectRepository;
}

std::shared_ptr<IModuleRepository> Container::getModuleRepository()
{
    if (!moduleRepository && database)
    {
        moduleRepository = std::make_shared<ModuleRepository>(*database);
    }
    return moduleRepository;
}

std::shared_ptr<INoteRepository> Container::getNoteRepository()
{
    if (!noteRepository && database)
    {
        noteRepository = std::make_shared<NoteRepository>(*database);
    }
    return noteRepository;
}