#ifndef MODULETEMPLATE_H
#define MODULETEMPLATE_H

#include "Model.h"
#include <QJsonObject>
#include <QString>

class ModuleTemplate : public Model
{
  public:
    ModuleTemplate() = default;

    QString getName() const
    {
        return name;
    }

    QString getCommand() const
    {
        return command;
    }

    int getPort() const
    {
        return port;
    }

    QString getDescription() const
    {
        return description;
    }

    QString getParameters() const
    {
        return parameters;
    }

    QString getEnvironment() const
    {
        return environment;
    }

    bool isEnabled() const
    {
        return enabled;
    }

    void setName(const QString& name)
    {
        this->name = name;
    }

    void setCommand(const QString& command)
    {
        this->command = command;
    }

    void setPort(int port)
    {
        this->port = port;
    }

    void setDescription(const QString& description)
    {
        this->description = description;
    }

    void setParameters(const QString& parameters)
    {
        this->parameters = parameters;
    }

    void setEnvironment(const QString& environment)
    {
        this->environment = environment;
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
        obj["command"] = command;
        obj["port"] = port;
        obj["description"] = description;
        obj["parameters"] = parameters;
        obj["environment"] = environment;
        obj["enabled"] = enabled;
        return obj;
    }

  private:
    QString name;
    QString command;
    int port = 0;
    QString description;
    QString parameters;
    QString environment;
    bool enabled = true;
};

#endif // MODULETEMPLATE_H