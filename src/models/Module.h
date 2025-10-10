#ifndef MODULE_H
#define MODULE_H

#include "Model.h"
#include <QJsonObject>
#include <QString>

class Module : public Model
{
  public:
    enum class Status
    {
        Stopped,
        Starting,
        Running,
        Stopping,
        Error
    };

    enum class ServiceType
    {
        CUSTOM,
        BACKEND,
        FRONTEND,
        DATABASE,
        CACHE,
        INFRASTRUCTURE
    };

    Module() = default;

    int getProjectId() const
    {
        return projectId;
    }

    QString getName() const
    {
        return name;
    }

    int getPort() const
    {
        return port;
    }

    Status getStatus() const
    {
        return status;
    }

    QString getCommand() const
    {
        return command;
    }

    QString getWorkingDirectory() const
    {
        return workingDirectory;
    }

    QString getLogs() const
    {
        return logs;
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

    ServiceType getServiceType() const
    {
        return serviceType;
    }

    void setProjectId(int projectId)
    {
        this->projectId = projectId;
    }

    void setName(const QString& name)
    {
        this->name = name;
    }

    void setPort(int port)
    {
        this->port = port;
    }

    void setStatus(Status status)
    {
        this->status = status;
    }

    void setCommand(const QString& command)
    {
        this->command = command;
    }

    void setWorkingDirectory(const QString& workingDirectory)
    {
        this->workingDirectory = workingDirectory;
    }

    void setLogs(const QString& logs)
    {
        this->logs = logs;
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

    void setServiceType(ServiceType type)
    {
        this->serviceType = type;
    }

    QString getStatusString() const
    {
        switch (status)
        {
        case Status::Stopped:
            return "Stopped";
        case Status::Starting:
            return "Starting";
        case Status::Running:
            return "Running";
        case Status::Stopping:
            return "Stopping";
        case Status::Error:
            return "Error";
        default:
            return "Unknown";
        }
    }

    QString getServiceTypeString() const
    {
        switch (serviceType)
        {
        case ServiceType::BACKEND:
            return "Backend";
        case ServiceType::FRONTEND:
            return "Frontend";
        case ServiceType::DATABASE:
            return "Database";
        case ServiceType::CACHE:
            return "Cache";
        case ServiceType::INFRASTRUCTURE:
            return "Infrastructure";
        case ServiceType::CUSTOM:
        default:
            return "Custom";
        }
    }

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["projectId"] = projectId;
        obj["name"] = name;
        obj["port"] = port;
        obj["status"] = static_cast<int>(status);
        obj["command"] = command;
        obj["workingDirectory"] = workingDirectory;
        obj["logs"] = logs;
        obj["description"] = description;
        obj["parameters"] = parameters;
        obj["environment"] = environment;
        obj["serviceType"] = static_cast<int>(serviceType);
        obj["createdAt"] = createdAt.toString(Qt::ISODate);
        obj["updatedAt"] = updatedAt.toString(Qt::ISODate);
        return obj;
    }

  private:
    int projectId = 0;
    QString name;
    int port = 0;
    Status status = Status::Stopped;
    QString command;
    QString workingDirectory;
    QString logs;
    QString description;
    QString parameters;
    QString environment;
    ServiceType serviceType = ServiceType::CUSTOM;
};

#endif // MODULE_H
