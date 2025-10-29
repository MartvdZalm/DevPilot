// clang-format off

#include "../../src/repositories/EditorRepository.h"
#include "../../src/models/Editor.h"
#include "../helpers/TestHelpers.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <catch2/catch_test_macros.hpp>

struct EditorRepoFixture
{
    QSqlDatabase db;
    std::unique_ptr<EditorRepository> repository;

    EditorRepoFixture()
    {
        // In-memory SQLite DB
        db = QSqlDatabase::addDatabase("QSQLITE", "editor_test_connection");
        db.setDatabaseName(":memory:");
        REQUIRE(db.open());

        QSqlQuery query(db);
        bool success = query.exec(R"(
            CREATE TABLE IF NOT EXISTS editors (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                path TEXT NOT NULL,
                arguments TEXT,
                enabled BOOLEAN NOT NULL DEFAULT 1,
                display_order INTEGER NOT NULL DEFAULT 0,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                UNIQUE(name)
            )
        )");
        REQUIRE(success);

        repository = std::make_unique<EditorRepository>(db);
    }

    ~EditorRepoFixture()
    {
        repository.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("editor_test_connection");
    }

    Editor createTestEditor(const QString& name = "Test Editor",
                            const QString& path = "/usr/bin/editor",
                            const QString& args = "--help",
                            bool enabled = true,
                            int displayOrder = 0)
    {
        Editor editor;
        editor.setName(name);
        editor.setPath(path);
        editor.setArguments(args);
        editor.setEnabled(enabled);
        editor.setDisplayOrder(displayOrder);
        return editor;
    }
};


TEST_CASE_METHOD(EditorRepoFixture, "Insert and find editor by ID", "[repository][insert][findById]")
{
    ARRANGE(
        Editor editor = createTestEditor("VSCode", "/usr/bin/code", "--wait", true, 1);
    )

    ACT(
        auto inserted = repository->save(editor);
    )

    ASSERT(
        REQUIRE(inserted.has_value());
        auto found = repository->findById(inserted->getId());
        REQUIRE(found.has_value());
        CHECK(found->getName() == "VSCode");
        CHECK(found->getPath() == "/usr/bin/code");
        CHECK(found->getArguments() == "--wait");
        CHECK(found->isEnabled() == true);
        CHECK(found->getDisplayOrder() == 1);
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "Find by ID returns nullopt for non-existent editor", "[repository][findById]")
{
    ARRANGE(
        int nonExistentId = 999;
    )

    ACT(
        auto found = repository->findById(nonExistentId);
    )

    ASSERT(
        CHECK_FALSE(found.has_value());
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "Update existing editor", "[repository][update]")
{
    ARRANGE(
        Editor editor = createTestEditor("OldName", "/old/path", "-o", true, 0);
        auto inserted = repository->save(editor);
        REQUIRE(inserted.has_value());

        Editor updated = *inserted;
        updated.setName("NewName");
        updated.setPath("/new/path");
        updated.setArguments("-n");
        updated.setEnabled(false);
        updated.setDisplayOrder(5);
    )

    ACT(
        auto result = repository->save(updated);
    )

    ASSERT(
        REQUIRE(result.has_value());
        auto found = repository->findById(inserted->getId());
        REQUIRE(found.has_value());
        CHECK(found->getName() == "NewName");
        CHECK(found->getPath() == "/new/path");
        CHECK(found->getArguments() == "-n");
        CHECK(found->isEnabled() == false);
        CHECK(found->getDisplayOrder() == 5);
        CHECK(found->getId() == inserted->getId());
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "FindAll returns empty list when no editors", "[repository][findAll]")
{
    ARRANGE()

    ACT(
        auto editors = repository->findAll();
    )

    ASSERT(
        CHECK(editors.isEmpty());
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "FindAll returns all editors", "[repository][findAll]")
{
    ARRANGE(
        repository->save(createTestEditor("Editor1"));
        repository->save(createTestEditor("Editor2"));
        repository->save(createTestEditor("Editor3"));
    )

    ACT(
        auto editors = repository->findAll();
    )

    ASSERT(
        REQUIRE(editors.size() == 3);
        CHECK(editors[0].getName() == "Editor1");
        CHECK(editors[1].getName() == "Editor2");
        CHECK(editors[2].getName() == "Editor3");
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "Delete existing editor", "[repository][delete]")
{
    ARRANGE(
        Editor editor = createTestEditor();
        auto inserted = repository->save(editor);
        REQUIRE(inserted.has_value());
        int id = inserted->getId();
    )

    ACT(
        bool deleted = repository->deleteById(id);
    )

    ASSERT(
        REQUIRE(deleted);
        auto found = repository->findById(id);
        CHECK_FALSE(found.has_value());
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "Delete non-existent editor returns true", "[repository][delete]")
{
    ARRANGE(
        int nonExistentId = 999;
    )

    ACT(
        bool result = repository->deleteById(nonExistentId);
    )

    ASSERT(
        CHECK(result);
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "Multiple inserts generate sequential IDs", "[repository][insert]")
{
    ARRANGE(
        auto editor1 = repository->save(createTestEditor("First"));
        auto editor2 = repository->save(createTestEditor("Second"));
        auto editor3 = repository->save(createTestEditor("Third"));
    )

    ACT()

    ASSERT(
        REQUIRE(editor1.has_value());
        REQUIRE(editor2.has_value());
        REQUIRE(editor3.has_value());
        CHECK(editor2->getId() > editor1->getId());
        CHECK(editor3->getId() > editor2->getId());
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "FindAll maintains insertion order", "[repository][findAll]")
{
    ARRANGE(
        repository->save(createTestEditor("Alpha"));
        repository->save(createTestEditor("Beta"));
        repository->save(createTestEditor("Gamma"));
    )

    ACT(
        auto editors = repository->findAll();
    )

    ASSERT(
        REQUIRE(editors.size() == 3);
        CHECK(editors[0].getName() == "Alpha");
        CHECK(editors[1].getName() == "Beta");
        CHECK(editors[2].getName() == "Gamma");
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "Save with ID 0 performs insert", "[repository][save]")
{
    ARRANGE(
        Editor editor = createTestEditor();
        editor.setId(0);
    )

    ACT(
        auto result = repository->save(editor);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() > 0);
    )
}

TEST_CASE_METHOD(EditorRepoFixture, "Save with positive ID performs update", "[repository][save]")
{
    ARRANGE(
        auto inserted = repository->save(createTestEditor("Original"));
        REQUIRE(inserted.has_value());

        Editor updated = *inserted;
        updated.setName("Modified");
    )

    ACT(
        auto result = repository->save(updated);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() == inserted->getId());
        CHECK(result->getName() == "Modified");
    )
}
