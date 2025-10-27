#ifndef ANSIHTMLCONVERTER_H
#define ANSIHTMLCONVERTER_H

#include <QMap>
#include <QString>

class AnsiHtmlConverter
{
  public:
    static QString toHtml(const QString& text);

  private:
    static QMap<QString, QString> foregroundColors();
    static QMap<QString, QString> backgroundColors();
};

#endif // ANSIHTMLCONVERTER_H
