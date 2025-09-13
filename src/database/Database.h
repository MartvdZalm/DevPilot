#ifndef DATABASE_H
#define DATABASE_H

#include <QMap>
#include <QMutex>
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
        return m_initialized;
    }
    QSqlDatabase& database()
    {
        return m_db;
    }

    bool execute(const QString& query, const QVariantMap& params = QVariantMap());

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

  private:
    Database() = default;
    ~Database();

    bool createTables();
    bool applyPragmas();
    bool verifyDatabase();

    QSqlDatabase m_db;
    QMutex m_mutex;
    bool m_initialized = false;
    static QMutex s_instanceMutex;
};

#endif // DATABASE_H