#ifndef MODEL_H
#define MODEL_H

#include <QDateTime>

class Model
{
  public:
    Model() = default;

    int getId() const
    {
        return id;
    }

    QDateTime getCreatedAt() const
    {
        return createdAt;
    }

    QDateTime getUpdatedAt() const
    {
        return updatedAt;
    }

    void setId(int id)
    {
        this->id = id;
    }

    void setCreatedAt(const QDateTime& createdAt)
    {
        this->createdAt = createdAt;
    }

    void setUpdatedAt(const QDateTime& updatedAt)
    {
        this->updatedAt = updatedAt;
    }

  protected:
    int id = 0;
    QDateTime createdAt;
    QDateTime updatedAt;
};

#endif // MODEL_H