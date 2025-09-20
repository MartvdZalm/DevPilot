#ifndef SETTINGREPOSITORY_H
#define SETTINGREPOSITORY_H

#include "ISettingRepository.h"
#include <QSqlDatabase>

class SettingRepository : public ISettingRepository
{
  public:
    explicit SettingRepository(QSqlDatabase& db);

    std::optional<Setting> findById(int id) override;
    QList<Setting> findAll() override;
    std::optional<Setting> save(const Setting& setting) override;
    bool deleteById(int id) override;
    std::optional<Setting> findByKey(const QString& key) override;
    QString getValue(const QString& key, const QString& defaultValue = "") override;
    std::optional<Setting> setValue(const QString& key, const QString& value) override;
    bool deleteByKey(const QString& key) override;
    Setting mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<Setting> insert(const Setting& setting);
    std::optional<Setting> update(const Setting& setting);
};

#endif // SETTINGREPOSITORY_H