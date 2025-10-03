#include "Seeder.h"
#include <QDebug>

void Seeder::addSeeder(std::unique_ptr<ISeeder> seeder)
{
    seeders.push_back(std::move(seeder));
}

bool Seeder::runSeeders()
{
    qDebug() << "Running database seeders...";

    bool allSuccess = true;
    int seededCount = 0;

    for (const auto& seeder : seeders)
    {
        if (seeder->shouldSeed())
        {
            qDebug() << "Running seeder:" << seeder->getName();
            if (seeder->seed())
            {
                seededCount++;
                qDebug() << "Seeder completed successfully:" << seeder->getName();
            }
            else
            {
                qWarning() << "Seeder failed:" << seeder->getName();
                allSuccess = false;
            }
        }
        else
        {
            qDebug() << "Seeder already run, skipping:" << seeder->getName();
        }
    }

    qDebug() << "Database seeding completed. Successfully ran" << seededCount << "seeders";
    return allSuccess;
}