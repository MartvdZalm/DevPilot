#ifndef CONTAINER_H
#define CONTAINER_H

#include "../repositories/IModuleRepository.h"
#include "../repositories/INoteRepository.h"
#include "../repositories/IProjectRepository.h"
#include <QSqlDatabase>
#include <memory>

class Container
{
  public:
    static Container& instance();

    void initialize(QSqlDatabase& database);

    std::shared_ptr<IProjectRepository> getProjectRepository();
    std::shared_ptr<IModuleRepository> getModuleRepository();
    std::shared_ptr<INoteRepository> getNoteRepository();

  private:
    Container() = default;

    QSqlDatabase* database = nullptr;
    std::shared_ptr<IProjectRepository> projectRepository;
    std::shared_ptr<IModuleRepository> moduleRepository;
    std::shared_ptr<INoteRepository> noteRepository;
};

#endif // CONTAINER_H
