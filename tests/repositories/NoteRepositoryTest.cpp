// clang-format off

#include "../../src/repositories/NoteRepository.h"
#include "../../src/models/Note.h"
#include "../helpers/TestHelpers.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <catch2/catch_test_macros.hpp>

struct NoteRepoFixture
{
    QSqlDatabase db;
    std::unique_ptr<NoteRepository> repository;

    NoteRepoFixture()
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "note_test_connection");
        db.setDatabaseName(":memory:");
        REQUIRE(db.open());

        QSqlQuery query(db);
        bool success = query.exec(R"(
            CREATE TABLE IF NOT EXISTS notes (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                project_id INTEGER NOT NULL,
                title TEXT,
                content TEXT NOT NULL,
                created_at DATETIME NOT NULL,
                updated_at DATETIME NOT NULL,
                FOREIGN KEY (project_id) REFERENCES projects (id) ON DELETE CASCADE
            )
        )");
        REQUIRE(success);

        repository = std::make_unique<NoteRepository>(db);
    }

    ~NoteRepoFixture()
    {
        repository.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("note_test_connection");
    }

    Note createTestNote(int projectId = 1,
                        const QString& title = "Test Note",
                        const QString& content = "This is a test note content")
    {
        Note note;
        note.setProjectId(projectId);
        note.setTitle(title);
        note.setContent(content);
        return note;
    }
};

TEST_CASE_METHOD(NoteRepoFixture, "Insert and find note by ID", "[repository][insert][findById]")
{
    ARRANGE(
        Note note = createTestNote(1, "Meeting Notes", "Discussed architecture updates");
    )

    ACT(
        auto inserted = repository->save(note);
    )

    ASSERT(
        REQUIRE(inserted.has_value());
        auto found = repository->findById(inserted->getId());
        REQUIRE(found.has_value());
        CHECK(found->getTitle() == "Meeting Notes");
        CHECK(found->getContent() == "Discussed architecture updates");
        CHECK(found->getProjectId() == 1);
    )
}

TEST_CASE_METHOD(NoteRepoFixture, "Find by ID returns nullopt for non-existent note", "[repository][findById]")
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

TEST_CASE_METHOD(NoteRepoFixture, "Find by project ID returns correct notes", "[repository][findByProjectId]")
{
    ARRANGE(
        repository->save(createTestNote(10, "Project10_Note1", "Content1"));
        repository->save(createTestNote(10, "Project10_Note2", "Content2"));
        repository->save(createTestNote(20, "Project20_Note", "Content3"));
    )

    ACT(
        auto notes = repository->findByProjectId(10);
    )

    ASSERT(
        REQUIRE(notes.size() == 2);
        CHECK(notes[0].getProjectId() == 10);
        CHECK(notes[1].getProjectId() == 10);
        CHECK(notes[0].getTitle() == "Project10_Note1");
        CHECK(notes[1].getTitle() == "Project10_Note2");
    )
}

TEST_CASE_METHOD(NoteRepoFixture, "Update existing note", "[repository][update]")
{
    ARRANGE(
        Note note = createTestNote(5, "Old Title", "Old content");
        auto inserted = repository->save(note);
        REQUIRE(inserted.has_value());

        Note updated = *inserted;
        updated.setTitle("New Title");
        updated.setContent("Updated content");
        updated.setProjectId(6);
    )

    ACT(
        auto result = repository->save(updated);
    )

    ASSERT(
        REQUIRE(result.has_value());
        auto found = repository->findById(updated.getId());
        REQUIRE(found.has_value());
        CHECK(found->getTitle() == "New Title");
        CHECK(found->getContent() == "Updated content");
        CHECK(found->getProjectId() == 6);
    )
}

TEST_CASE_METHOD(NoteRepoFixture, "FindAll returns all notes", "[repository][findAll]")
{
    ARRANGE(
        repository->save(createTestNote(1, "Note1", "Content1"));
        repository->save(createTestNote(2, "Note2", "Content2"));
    )

    ACT(
        auto notes = repository->findAll();
    )

    ASSERT(
        REQUIRE(notes.size() == 2);
        CHECK(notes[0].getTitle() == "Note1");
        CHECK(notes[1].getTitle() == "Note2");
    )
}

TEST_CASE_METHOD(NoteRepoFixture, "FindAll returns empty list when no notes", "[repository][findAll]")
{
    ARRANGE()

    ACT(
        auto notes = repository->findAll();
    )

    ASSERT(
        CHECK(notes.isEmpty());
    )
}

TEST_CASE_METHOD(NoteRepoFixture, "Delete existing note", "[repository][delete]")
{
    ARRANGE(
        Note note = createTestNote();
        auto inserted = repository->save(note);
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

TEST_CASE_METHOD(NoteRepoFixture, "Delete non-existent note returns true", "[repository][delete]")
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

TEST_CASE_METHOD(NoteRepoFixture, "Multiple inserts generate sequential IDs", "[repository][insert]")
{
    ARRANGE(
        auto note1 = repository->save(createTestNote(1, "First", "A"));
        auto note2 = repository->save(createTestNote(1, "Second", "B"));
        auto note3 = repository->save(createTestNote(1, "Third", "C"));
    )

    ASSERT(
        REQUIRE(note1.has_value());
        REQUIRE(note2.has_value());
        REQUIRE(note3.has_value());
        CHECK(note2->getId() > note1->getId());
        CHECK(note3->getId() > note2->getId());
    )
}

TEST_CASE_METHOD(NoteRepoFixture, "Save with ID 0 performs insert", "[repository][save]")
{
    ARRANGE(
        Note note = createTestNote(2, "InsertTest", "Should insert");
        note.setId(0);
    )

    ACT(
        auto result = repository->save(note);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() > 0);
        CHECK(result->getTitle() == "InsertTest");
    )
}

TEST_CASE_METHOD(NoteRepoFixture, "Save with positive ID performs update", "[repository][save]")
{
    ARRANGE(
        auto inserted = repository->save(createTestNote(3, "Original", "Before update"));
        REQUIRE(inserted.has_value());

        Note updated = *inserted;
        updated.setTitle("Modified");
        updated.setContent("After update");
    )

    ACT(
        auto result = repository->save(updated);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() == inserted->getId());
        CHECK(result->getTitle() == "Modified");
        CHECK(result->getContent() == "After update");
    )
}
