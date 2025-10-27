#include "AnsiHtmlConverter.h"
#include <QRegularExpression>

QMap<QString, QString> AnsiHtmlConverter::foregroundColors()
{
    return {{"30", "black"},     {"31", "red"},        {"32", "green"},      {"33", "yellow"},
            {"34", "blue"},      {"35", "magenta"},    {"36", "cyan"},       {"37", "white"},
            {"90", "gray"},      {"91", "lightcoral"}, {"92", "lightgreen"}, {"93", "lightyellow"},
            {"94", "lightblue"}, {"95", "violet"},     {"96", "cyan"},       {"97", "white"}};
}

QMap<QString, QString> AnsiHtmlConverter::backgroundColors()
{
    return {{"40", "black"}, {"41", "red"},     {"42", "green"}, {"43", "yellow"},
            {"44", "blue"},  {"45", "magenta"}, {"46", "cyan"},  {"47", "white"}};
}

QString AnsiHtmlConverter::toHtml(const QString& text)
{
    QString html = text;

    // 1. First, escape HTML entities
    html.replace("&", "&amp;");
    html.replace("<", "&lt;");
    html.replace(">", "&gt;");

    // 2. Apply ANSI color codes FIRST
    QRegularExpression ansiRegex("\x1B\\[([0-9;]+)m");
    int offset = 0;
    QString result;
    int lastPos = 0;

    QMap<QString, QString> fg = foregroundColors();
    QMap<QString, QString> bg = backgroundColors();

    bool inSpan = false;

    while (true)
    {
        QRegularExpressionMatch match = ansiRegex.match(html, offset);
        if (!match.hasMatch())
        {
            break;
        }

        int matchStart = match.capturedStart();
        int matchEnd = match.capturedEnd();

        result += html.mid(lastPos, matchStart - lastPos);

        QStringList codes = match.captured(1).split(';');
        QString style;

        for (const QString& code : codes)
        {
            if (code == "0")
            {
                if (inSpan)
                {
                    result += "</span>";
                    inSpan = false;
                }
                style = "";
                break;
            }
            if (fg.contains(code))
                style += "color:" + fg[code] + ";";
            if (bg.contains(code))
                style += "background-color:" + bg[code] + ";";
            if (code == "1")
                style += "font-weight:bold;";
            if (code == "3")
                style += "font-style:italic;";
        }

        if (!style.isEmpty())
        {
            if (inSpan)
            {
                result += "</span>";
            }
            result += "<span style=\"" + style + "\">";
            inSpan = true;
        }

        lastPos = matchEnd;
        offset = matchEnd;
    }

    result += html.mid(lastPos);

    if (inSpan)
    {
        result += "</span>";
    }

    // 3. Now detect and linkify URLs
    QRegularExpression urlRegex(R"(\b(https?://[^\s&lt;&gt;&quot;]+))");
    result.replace(urlRegex, R"(<a href="\1" style="color: #1E90FF; text-decoration: underline;">\1</a>)");

    // 4. Finally convert newlines
    result.replace("\n", "<br>");

    return result;
}
