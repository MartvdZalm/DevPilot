#ifndef DATABASE_H
#define DATABASE_H

#include <QMap>
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QVariant>

class Database : public QObject
{
    Q_OBJECT

  public:
    static Database& instance();

    bool initialize();

    bool isInitialized() const
    {
        return initialized;
    }

    QSqlDatabase& getDatabase()
    {
        return db;
    }

    bool execute(const QString& query, const QVariantMap& params = QVariantMap());

  private:
    ~Database();
    bool createTables();
    bool applyPragmas();
    bool verifyDatabase();

  private:
    QSqlDatabase db;
    bool initialized = false;
};

#endif // DATABASE_H
