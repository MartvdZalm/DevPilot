#include "Seeder.h"
#include "../../core/Logger.h"
#include <QDebug>

void Seeder::addSeeder(std::unique_ptr<ISeeder> seeder)
{
    seeders.push_back(std::move(seeder));
}

bool Seeder::runSeeders()
{
    LOG_INFO("Running database seeders...");

    bool allSuccess = true;
    int seededCount = 0;

    for (const auto& seeder : seeders)
    {
        if (seeder->shouldSeed())
        {
            LOG_INFO("Running seeder: " + seeder->getName());
            if (seeder->seed())
            {
                seededCount++;
                LOG_INFO("Seeder completed successfully: " + seeder->getName());
            }
            else
            {
                LOG_WARNING("Seeder failed: " + seeder->getName());
                allSuccess = false;
            }
        }
        else
        {
            LOG_INFO("Seeder already run, skipping: " + seeder->getName());
        }
    }

    LOG_INFO("Database seeding completed. Successfully ran " + QString::number(seededCount) + " seeders");
    return allSuccess;
}
