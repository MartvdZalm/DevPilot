#ifndef EDITOR_H
#define EDITOR_H

#include "Model.h"
#include <QJsonObject>
#include <QDateTime>
#include <QString>

class Editor : public Model
{
  public:
    Editor() = default;

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

    int getDisplayOrder() const
    {
        return displayOrder;
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

    void setDisplayOrder(int order)
    {
        this->displayOrder = order;
    }

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["path"] = path;
        obj["arguments"] = arguments;
        obj["enabled"] = enabled;
        obj["displayOrder"] = displayOrder;
        obj["createdAt"] = createdAt.toString(Qt::ISODate);
        obj["updatedAt"] = updatedAt.toString(Qt::ISODate);
        return obj;
    }

  private:
    QString name;
    QString path;
    QString arguments;
    bool enabled = true;
    int displayOrder = 0;
};

#endif // EDITOR_H