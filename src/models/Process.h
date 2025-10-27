#ifndef PROCESS_H
#define PROCESS_H

#include "Model.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

class Process : public Model
{
  public:
    enum class Status
    {
        Stopped,
        Starting,
        Running,
        Error
    };

    Process() = default;

    int getProjectId() const
    {
        return projectId;
    }

    QString getName() const
    {
        return name;
    }

    QString getCommand() const
    {
        return command;
    }

    QString getWorkingDirectory() const
    {
        return workingDirectory;
    }

    Status getStatus() const
    {
        return status;
    }

    int getPID() const
    {
        return pid;
    }

    int getPort() const
    {
        return port;
    }

    QString getLogPath() const
    {
        return logPath;
    }

    QDateTime getLastStartedAt() const
    {
        return lastStartedAt;
    }

    QDateTime getUptime() const
    {
        return uptime;
    }

    void setProjectId(int projectId)
    {
        this->projectId = projectId;
    }

    void setName(const QString& name)
    {
        this->name = name;
    }

    void setCommand(const QString& command)
    {
        this->command = command;
    }

    void setWorkingDirectory(const QString& workingDirectory)
    {
        this->workingDirectory = workingDirectory;
    }

    void setStatus(Status status)
    {
        this->status = status;
    }

    void setPID(int pid)
    {
        this->pid = pid;
    }

    void setPort(int port)
    {
        this->port = port;
    }

    void setLogPath(const QString& logPath)
    {
        this->logPath = logPath;
    }

    void setLastStartedAt(const QDateTime& lastStartedAt)
    {
        this->lastStartedAt = lastStartedAt;
    }

    void setUptime(const QDateTime& uptime)
    {
        this->uptime = uptime;
    }

    static Status fromString(const QString& statusStr)
    {
        QString s = statusStr.trimmed().toLower();
        if (s == "stopped")
            return Status::Stopped;
        if (s == "starting")
            return Status::Starting;
        if (s == "running")
            return Status::Running;
        if (s == "error")
            return Status::Error;
        return Status::Stopped;
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
        case Status::Error:
            return "Error";
        default:
            return "Unknown";
        }
    }

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["projectId"] = projectId;
        obj["name"] = name;
        obj["command"] = command;
        obj["workingDirectory"] = workingDirectory;
        obj["status"] = static_cast<int>(status);
        obj["pid"] = pid;
        obj["port"] = port;
        obj["logPath"] = logPath;
        obj["lastStartedAt"] = lastStartedAt.toString(Qt::ISODate);
        obj["uptime"] = uptime.toString(Qt::ISODate);
        obj["updatedAt"] = updatedAt.toString(Qt::ISODate);
        obj["createdAt"] = createdAt.toString(Qt::ISODate);
        return obj;
    }

  private:
    int projectId = 0;
    QString name;
    QString command;
    QString workingDirectory;
    Status status = Status::Stopped;
    int pid = 0;
    int port = 0;
    QString logPath;
    QDateTime lastStartedAt;
    QDateTime uptime;
};

#endif // PROCESS_H
