#ifndef APP_H
#define APP_H

#include "Model.h"
#include <QJsonObject>
#include <QString>

class App : public Model
{
  public:
    App() = default;

    QString getName() const
    {
        return name;
    }

    QString getPath() const
    {
        return path;
    }

    QString getArguments() const
    {
        return arguments;
    }

    bool isEnabled() const
    {
        return enabled;
    }

    void setName(const QString& name)
    {
        this->name = name;
    }

    void setPath(const QString& path)
    {
        this->path = path;
    }

    void setArguments(const QString& arguments)
    {
        this->arguments = arguments;
    }

    void setEnabled(bool enabled)
    {
        this->enabled = enabled;
    }

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["path"] = path;
        obj["arguments"] = arguments;
        obj["enabled"] = enabled;
        obj["createdAt"] = createdAt.toString(Qt::ISODate);
        obj["updatedAt"] = updatedAt.toString(Qt::ISODate);
        return obj;
    }

  private:
    QString name;
    QString path;
    QString arguments;
    bool enabled = true;
};

#endif // APP_H
