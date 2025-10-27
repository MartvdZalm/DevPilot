#ifndef SNIPPET_H
#define SNIPPET_H

#include "Model.h"
#include <QString>
#include <QDateTime>

class Snippet  : public Model
{
  public:
    Snippet() = default;

    QString getTitle() const
    {
        return title;
    }

    QString getLanguage() const
    {
        return language;
    }

    QString getCode() const
    {
        return code;
    }

    QString getDescription() const
    {
        return description;
    }

    void setTitle(const QString& title)
    {
        this->title = title;
    }

    void setLanguage(const QString& language)
    {
        this->language = language;
    }

    void setCode(const QString& code)
    {
        this->code = code;
    }

    void setDescription(const QString& description)
    {
        this->description = description;
    }

    int getLineCount() const
    {
        if (code.isEmpty()) {
            return 0;
        }
        return code.count('\n') + 1;
    }

    int getCharacterCount() const
    {
        return code.length();
    }

    bool isEmpty() const
    {
        return title.isEmpty() && code.isEmpty();
    }

  private:
    QString title;
    QString language;
    QString code;
    QString description;
};

#endif // SNIPPET_H
