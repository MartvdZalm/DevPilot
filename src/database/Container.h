#ifndef CONTAINER_H
#define CONTAINER_H

#include "../repositories/IModuleRepository.h"
#include "../repositories/INoteRepository.h"
#include "../repositories/IProjectRepository.h"
#include "../repositories/ISettingRepository.h"
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
    std::shared_ptr<ISettingRepository> getSettingRepository();

  private:
    Container() = default;

    QSqlDatabase* database = nullptr;
    std::shared_ptr<IProjectRepository> projectRepository;
    std::shared_ptr<IModuleRepository> moduleRepository;
    std::shared_ptr<INoteRepository> noteRepository;
    std::shared_ptr<ISettingRepository> settingRepository;
};

#endif // CONTAINER_H
