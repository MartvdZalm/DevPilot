#ifndef PROJECT_H
#define PROJECT_H

#include "Model.h"
#include <QJsonObject>
#include <QString>

class Project : public Model
{
  public:
    Project() = default;

    QString getName() const
    {
        return name;
    }

    QString getDirectoryPath() const
    {
        return directoryPath;
    }

    QString getDescription() const
    {
        return description;
    }

    QDateTime getLastOpenedAt() const
    {
        return lastOpenedAt;
    }

    void setName(const QString& name)
    {
        this->name = name;
    }

    void setDirectoryPath(const QString& directoryPath)
    {
        this->directoryPath = directoryPath;
    }

    void setDescription(const QString& description)
    {
        this->description = description;
    }

    void setLastOpenedAt(const QDateTime& lastOpenedAt)
    {
        this->lastOpenedAt = lastOpenedAt;
    }

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["directoryPath"] = directoryPath;
        obj["description"] = description;
        obj["lastOpenedAt"] = lastOpenedAt.toString(Qt::ISODate);
        obj["createdAt"] = createdAt.toString(Qt::ISODate);
        obj["updatedAt"] = updatedAt.toString(Qt::ISODate);
        return obj;
    }

  private:
    QString name;
    QString directoryPath;
    QString description;
    QDateTime lastOpenedAt;
};

#endif // PROJECT_H
