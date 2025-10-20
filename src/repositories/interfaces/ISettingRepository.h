#ifndef ISETTINGREPOSITORY_H
#define ISETTINGREPOSITORY_H

#include "../../models/Setting.h"
#include <QSqlQuery>
#include <QString>
#include <optional>

class ISettingRepository
{
  public:
    virtual ~ISettingRepository() = default;

    virtual std::optional<Setting> findById(int id) = 0;
    virtual QList<Setting> findAll() = 0;
    virtual std::optional<Setting> save(const Setting& setting) = 0;
    virtual bool deleteById(int id) = 0;

    virtual std::optional<Setting> findByKey(const QString& key) = 0;
    virtual QString getValue(const QString& key, const QString& defaultValue = "") = 0;
    virtual std::optional<Setting> setValue(const QString& key, const QString& value) = 0;
    virtual bool deleteByKey(const QString& key) = 0;
    virtual Setting mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // ISETTINGREPOSITORY_H
