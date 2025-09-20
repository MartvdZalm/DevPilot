#ifndef SETTING_H
#define SETTING_H

#include "Model.h"
#include <QString>
#include <QJsonObject>

class Setting : public Model
{
  public:
    Setting() = default;

    QString getKey() const
    {
        return key;
    }

    QString getValue() const
    {
        return value;
    }

    void setKey(const QString& key)
    {
        this->key = key;
    }

    void setValue(const QString& value)
    {
        this->value = value;
    }

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["key"] = key;
        obj["value"] = value;
        return obj;
    }

  private:
    QString key;
    QString value;
};

#endif // SETTING_H