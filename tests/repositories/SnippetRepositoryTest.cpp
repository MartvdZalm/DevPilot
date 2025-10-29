// clang-format off

#include "../../src/repositories/SnippetRepository.h"
#include "../../src/models/Snippet.h"
#include "../helpers/TestHelpers.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <catch2/catch_test_macros.hpp>

struct SnippetRepoFixture
{
    QSqlDatabase db;
    std::unique_ptr<SnippetRepository> repository;

    SnippetRepoFixture()
    {
        // In-memory SQLite DB
        db = QSqlDatabase::addDatabase("QSQLITE", "test_connection");
        db.setDatabaseName(":memory:");
        REQUIRE(db.open());

        QSqlQuery query(db);
        bool success = query.exec(R"(
            CREATE TABLE IF NOT EXISTS snippets (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT NOT NULL,
                language TEXT NOT NULL,
                code TEXT NOT NULL,
                description TEXT,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        )");
        REQUIRE(success);

        repository = std::make_unique<SnippetRepository>(db);
    }

    ~SnippetRepoFixture()
    {
        repository.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("test_connection");
    }

    Snippet createTestSnippet(const QString& title = "Test Snippet",
                              const QString& language = "C++",
                              const QString& code = "int main() {}",
                              const QString& description = "Test description")
    {
        Snippet snippet;
        snippet.setTitle(title);
        snippet.setLanguage(language);
        snippet.setCode(code);
        snippet.setDescription(description);
        return snippet;
    }
};

TEST_CASE_METHOD(SnippetRepoFixture, "Insert and find snippet by ID", "[repository][insert][findById]")
{
    ARRANGE(
        Snippet snippet = createTestSnippet("Hello World", "C++", "std::cout << \"Hello\";", "Simple test snippet");
    )

    ACT(
        auto inserted = repository->save(snippet);
        auto found = repository->findById(inserted->getId());
    )

    ASSERT(
        REQUIRE(inserted.has_value());
        REQUIRE(inserted->getId() > 0);

        REQUIRE(found.has_value());
        CHECK(found->getTitle() == "Hello World");
        CHECK(found->getLanguage() == "C++");
        CHECK(found->getCode() == "std::cout << \"Hello\";");
        CHECK(found->getDescription() == "Simple test snippet");
    )
}

TEST_CASE_METHOD(SnippetRepoFixture, "Find by ID returns nullopt for non-existent snippet", "[repository][findById]")
{
    ACT(
        auto found = repository->findById(999);
    )

    ASSERT(
        CHECK_FALSE(found.has_value());
    )
}

TEST_CASE_METHOD(SnippetRepoFixture, "Update existing snippet", "[repository][update]")
{
    ARRANGE(
        Snippet snippet = createTestSnippet("Original Title", "Python", "print('hello')", "Original description");
        auto inserted = repository->save(snippet);
        REQUIRE(inserted.has_value());

        Snippet updated = *inserted;
        updated.setTitle("Updated Title");
        updated.setLanguage("JavaScript");
        updated.setCode("console.log('hello');");
        updated.setDescription("Updated description");
    )

    ACT(
        auto result = repository->save(updated);
        auto found = repository->findById(inserted->getId());
    )

    ASSERT(
        REQUIRE(result.has_value());

        REQUIRE(found.has_value());
        CHECK(found->getTitle() == "Updated Title");
        CHECK(found->getLanguage() == "JavaScript");
        CHECK(found->getCode() == "console.log('hello');");
        CHECK(found->getDescription() == "Updated description");
        CHECK(found->getId() == inserted->getId());
    )
}


TEST_CASE_METHOD(SnippetRepoFixture, "FindAll returns empty list when no snippets", "[repository][findAll]")
{
    ACT(
        auto snippets = repository->findAll();
    )

    ASSERT(
        CHECK(snippets.isEmpty());
    )
}

TEST_CASE_METHOD(SnippetRepoFixture, "FindAll returns all snippets", "[repository][findAll]")
{
    ARRANGE(
        repository->save(createTestSnippet("Snippet 1", "C++"));
        repository->save(createTestSnippet("Snippet 2", "Python"));
        repository->save(createTestSnippet("Snippet 3", "JavaScript"));
    )

    ACT(
        auto snippets = repository->findAll();
    )

    ASSERT(
        REQUIRE(snippets.size() == 3);
        CHECK(snippets[0].getTitle() == "Snippet 1");
        CHECK(snippets[1].getTitle() == "Snippet 2");
        CHECK(snippets[2].getTitle() == "Snippet 3");
    )
}

TEST_CASE_METHOD(SnippetRepoFixture, "Delete existing snippet", "[repository][delete]")
{
    ARRANGE(
        Snippet snippet = createTestSnippet();
        auto inserted = repository->save(snippet);
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

TEST_CASE_METHOD(SnippetRepoFixture, "Delete non-existent snippet returns true", "[repository][delete]")
{
    ACT(
        bool result = repository->deleteById(999);
    )

    ASSERT(
        CHECK(result);
    )
}


TEST_CASE_METHOD(SnippetRepoFixture, "Insert snippet with empty fields", "[repository][insert][edge-case]")
{
    ARRANGE(
        Snippet snippet;
        snippet.setTitle("");
        snippet.setLanguage("");
        snippet.setCode("");
        snippet.setDescription("");
    )

    ACT(
        auto inserted = repository->save(snippet);
        auto found = repository->findById(inserted->getId());

    )

    ASSERT(
        REQUIRE(inserted.has_value());

        REQUIRE(found.has_value());
        CHECK(found->getTitle() == "");
        CHECK(found->getLanguage() == "");
        CHECK(found->getCode() == "");
        CHECK(found->getDescription() == "");
    )
}

TEST_CASE_METHOD(SnippetRepoFixture, "Insert snippet with special characters", "[repository][insert][edge-case]")
{
    ARRANGE(
        Snippet snippet = createTestSnippet(
            "Special \"Title\" with 'quotes'",
            "C++",
            "std::string s = \"Hello\\nWorld\";",
            "Description with\nnewlines and\ttabs"
            );
    )

    ACT(
        auto inserted = repository->save(snippet);
        auto found = repository->findById(inserted->getId());

    )

    ASSERT(
        REQUIRE(inserted.has_value());

        REQUIRE(found.has_value());
        CHECK(found->getTitle() == "Special \"Title\" with 'quotes'");
        CHECK(found->getCode() == "std::string s = \"Hello\\nWorld\";");
        CHECK(found->getDescription() == "Description with\nnewlines and\ttabs");
    )
}


TEST_CASE_METHOD(SnippetRepoFixture, "Insert snippet with very long content", "[repository][insert][edge-case]")
{
    ARRANGE(
        QString longCode(10000, 'x');  // 10,000 characters
        Snippet snippet = createTestSnippet("Long Code", "C++", longCode, "Test long content");
    )

    ACT(
        auto inserted = repository->save(snippet);
        auto found = repository->findById(inserted->getId());

    )

    ASSERT(
        REQUIRE(inserted.has_value());

        REQUIRE(found.has_value());
        CHECK(found->getCode().length() == 10000);
    )
}


TEST_CASE_METHOD(SnippetRepoFixture, "Multiple inserts generate sequential IDs", "[repository][insert]")
{
    ARRANGE(
        auto snippet1Data = createTestSnippet("First");
        auto snippet2Data = createTestSnippet("Second");
        auto snippet3Data = createTestSnippet("Third");
    )

    ACT(
        auto snippet1 = repository->save(snippet1Data);
        auto snippet2 = repository->save(snippet2Data);
        auto snippet3 = repository->save(snippet3Data);
    )

    ASSERT(
        REQUIRE(snippet1.has_value());
        REQUIRE(snippet2.has_value());
        REQUIRE(snippet3.has_value());
        CHECK(snippet2->getId() > snippet1->getId());
        CHECK(snippet3->getId() > snippet2->getId());
    )
}


TEST_CASE_METHOD(SnippetRepoFixture, "FindAll maintains insertion order", "[repository][findAll]")
{
    ARRANGE(
        auto alpha = createTestSnippet("Alpha");
        auto beta = createTestSnippet("Beta");
        auto gamma = createTestSnippet("Gamma");
    )

    ACT(
        repository->save(alpha);
        repository->save(beta);
        repository->save(gamma);
        auto snippets = repository->findAll();
    )

    ASSERT(
        REQUIRE(snippets.size() == 3);
        CHECK(snippets[0].getTitle() == "Alpha");
        CHECK(snippets[1].getTitle() == "Beta");
        CHECK(snippets[2].getTitle() == "Gamma");
    )
}

TEST_CASE_METHOD(SnippetRepoFixture, "Save with ID 0 performs insert", "[repository][save]")
{
    ARRANGE(
        Snippet snippet = createTestSnippet();
        snippet.setId(0);
    )

    ACT(
        auto result = repository->save(snippet);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() > 0);
    )
}

TEST_CASE_METHOD(SnippetRepoFixture, "Save with positive ID performs update", "[repository][save]")
{
    ARRANGE(
        auto inserted = repository->save(createTestSnippet("Original"));
        REQUIRE(inserted.has_value());

        Snippet updated = *inserted;
        updated.setTitle("Modified");
    )

    ACT(
        auto result = repository->save(updated);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() == inserted->getId());
        CHECK(result->getTitle() == "Modified");
    )
}

