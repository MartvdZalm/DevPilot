#include "Logger.h"

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>

QString Logger::logFilePath;
QMutex Logger::logMutex;

void Logger::initialize(const QString& appName)
{
    QMutexLocker locker(&logMutex);

    QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(logDir);

    logFilePath = logDir + "/" + appName + ".log";

    QFile file(logFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream(&file);
        stream << "\n\n=== Logging started at " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
               << " ===\n";
    }
}

void Logger::log(Level level, const QString& message, const char* file, const char* function, int line)
{
    QMutexLocker locker(&logMutex);

    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream(&logFile);
        stream << formatMessage(level, message, file, function, line) << "\n";
        logFile.close();
    }
}

QString Logger::formatMessage(Level level, const QString& message, const char* file, const char* function, int line)
{
    QString levelStr;
    switch (level)
    {
    case Level::Debug:
        levelStr = "DEBUG";
        break;
    case Level::Info:
        levelStr = "INFO";
        break;
    case Level::Warning:
        levelStr = "WARN";
        break;
    case Level::Error:
        levelStr = "ERROR";
        break;
    case Level::Critical:
        levelStr = "CRIT";
        break;
    }

    QString shortFile = QFileInfo(file ? file : "").fileName();

    return QString("[%1] [%2] [%3:%4@%5] %7")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(levelStr)
        .arg(shortFile)
        .arg(line)
        .arg(function ? function : "unknown")
        .arg(message);
}