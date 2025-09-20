#include "SettingRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

SettingRepository::SettingRepository(QSqlDatabase& db) : database(db) {}

std::optional<Setting> SettingRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM settings WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding setting ID " + QString::number(id) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto setting = mapFromRecord(query);
        LOG_INFO("Successfully found setting ID: " + id);
        return setting;
    }

    LOG_INFO("Setting not found with ID: " + id);
    return std::nullopt;
}

std::optional<Setting> SettingRepository::findByKey(const QString& key)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM settings WHERE key = :key");
    query.bindValue(":key", key);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding setting with key " + key + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        auto setting = mapFromRecord(query);
        LOG_INFO("Successfully found setting ID: " + setting.getId());
        return setting;
    }

    LOG_INFO("Setting not found with key: " + key);
    return std::nullopt;
}

QString SettingRepository::getValue(const QString& key, const QString& defaultValue)
{
    auto setting = findByKey(key);
    return setting ? setting->getValue() : defaultValue;
}

std::optional<Setting> SettingRepository::setValue(const QString& key, const QString& value)
{
    auto existing = findByKey(key);

    if (existing)
    {
        Setting updated = existing.value();
        updated.setValue(value);
        updated.setUpdatedAt(QDateTime::currentDateTime());
        return update(updated);
    }

    Setting newSetting;
    newSetting.setKey(key);
    newSetting.setValue(value);
    return insert(newSetting);
}

std::optional<Setting> SettingRepository::save(const Setting& setting)
{
    return setting.getId() > 0 ? update(setting) : insert(setting);
}

QList<Setting> SettingRepository::findAll()
{
    QList<Setting> results;
    QSqlQuery query("SELECT * FROM settings", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all settings: " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(results.size()) + "settings from database");
    return results;
}

bool SettingRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM settings WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete setting ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted setting ID: " + id);
    return query.exec();
}

bool SettingRepository::deleteByKey(const QString& key)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM settings WHERE key = ?");
    query.addBindValue(key);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete setting by key " + key + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully deleted setting key: " + key);
    return query.exec();
}

Setting SettingRepository::mapFromRecord(const QSqlQuery& query)
{
    Setting setting;
    setting.setId(query.value("id").toInt());
    setting.setKey(query.value("key").toString());
    setting.setValue(query.value("value").toString());
    setting.setCreatedAt(query.value("created_at").toDateTime());
    setting.setUpdatedAt(query.value("updated_at").toDateTime());
    return setting;
}

std::optional<Setting> SettingRepository::insert(const Setting& setting)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO settings (key, value) VALUES (:key, :value)");

    query.bindValue(":key", setting.getKey());
    query.bindValue(":value", setting.getValue());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert setting '" + setting.getKey() + "' : " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new setting ID: " + id);
    return findById(id);
}

std::optional<Setting> SettingRepository::update(const Setting& setting)
{
    QSqlQuery query(database);
    query.prepare("UPDATE settings SET key = :key, value = :value, updated_at = :updated_at WHERE id = :id");
    query.bindValue(":key", setting.getKey());
    query.bindValue(":value", setting.getValue());
    query.bindValue(":updated_at", setting.getUpdatedAt());
    query.bindValue(":id", setting.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update setting ID " + QString::number(setting.getId()) + " : " + query.lastError().text());
        return std::nullopt;
    }

    LOG_INFO("Successfully updated setting ID: " + setting.getId());
    return setting;
}