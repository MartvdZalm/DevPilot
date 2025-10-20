#ifndef ISEEDER_H
#define ISEEDER_H

#include <QString>

class ISeeder
{
  public:
    virtual ~ISeeder() = default;
    virtual bool shouldSeed() = 0;
    virtual bool seed() = 0;
    virtual QString getName() = 0;
};

#endif // ISEEDER_H