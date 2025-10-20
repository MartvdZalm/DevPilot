#ifndef SEEDER_H
#define SEEDER_H

#include "ISeeder.h"
#include <memory>
#include <vector>

class Seeder
{
  public:
    void addSeeder(std::unique_ptr<ISeeder> seeder);
    bool runSeeders();

  private:
    std::vector<std::unique_ptr<ISeeder>> seeders;
};

#endif // SEEDER_H