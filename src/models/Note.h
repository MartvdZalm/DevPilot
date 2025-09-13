#ifndef NOTE_H
#define NOTE_H

#include "Model.h"
#include <QJsonObject>
#include <QString>

class Note : public Model
{
  public:
    Note() = default;

    int getProjectId() const
    {
        return projectId;
    }

    QString getTitle() const
    {
        return title;
    }

    QString getContent() const
    {
        return content;
    }

    void setProjectId(int projectId)
    {
        this->projectId = projectId;
    }

    void setTitle(const QString& title)
    {
        this->title = title;
    }

    void setContent(const QString& content)
    {
        this->content = content;
    }

    QJsonObject serialize() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["title"] = title;
        obj["projectId"] = projectId;
        obj["content"] = content;
        obj["createdAt"] = createdAt.toString(Qt::ISODate);
        obj["updatedAt"] = updatedAt.toString(Qt::ISODate);
        return obj;
    }

  private:
    QString title;
    int projectId;
    QString content;
};

#endif // NOTE_H