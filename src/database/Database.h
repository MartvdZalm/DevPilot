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
    bool createProjectsTable(QSqlQuery& query);
    bool createProcessesTable(QSqlQuery& query);
    bool createNotesTable(QSqlQuery& query);
    bool createEditorsTable(QSqlQuery& query);
    bool createProcessTemplatesTable(QSqlQuery& query);
    bool createAppsTables(QSqlQuery& query);
    bool createSnippetTable(QSqlQuery& query);
    bool applyPragmas();
    bool verifyDatabase();
    bool runMigrations();
    bool migrateProcessesTable();

  private:
    QSqlDatabase db;
    bool initialized = false;
};

#endif // DATABASE_H
