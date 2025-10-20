#ifndef EDITORREPOSITORY_H
#define EDITORREPOSITORY_H

#include "interfaces/IEditorRepository.h"
#include <QSqlDatabase>

class EditorRepository : public IEditorRepository
{
  public:
    explicit EditorRepository(QSqlDatabase& db);

    std::optional<Editor> findById(int id) override;
    QList<Editor> findAll() override;
    std::optional<Editor> save(const Editor& editor) override;
    bool deleteById(int id) override;
    
    Editor mapFromRecord(const QSqlQuery& query) override;

  private:
    QSqlDatabase& database;
    std::optional<Editor> insert(const Editor& editor);
    std::optional<Editor> update(const Editor& editor);
};

#endif // EDITORREPOSITORY_H
