// clang-format off

#include "../../src/repositories/ProcessTemplateRepository.h"
#include "../../src/models/ProcessTemplate.h"
#include "../helpers/TestHelpers.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <catch2/catch_test_macros.hpp>

struct ProcessTemplateRepoFixture
{
    QSqlDatabase db;
    std::unique_ptr<ProcessTemplateRepository> repository;

    ProcessTemplateRepoFixture()
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "process_template_test_connection");
        db.setDatabaseName(":memory:");
        REQUIRE(db.open());

        QSqlQuery query(db);
        bool success = query.exec(R"(
            CREATE TABLE process_templates (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT,
                command TEXT,
                port INTEGER,
                description TEXT,
                parameters TEXT,
                environment TEXT,
                enabled BOOLEAN,
                created_at DATETIME,
                updated_at DATETIME
            )
        )");
        REQUIRE(success);

        repository = std::make_unique<ProcessTemplateRepository>(db);
    }

    ~ProcessTemplateRepoFixture()
    {
        repository.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("process_template_test_connection");
    }

    ProcessTemplate createTestTemplate(const QString& name = "Template1")
    {
        ProcessTemplate t;
        t.setName(name);
        t.setCommand("echo test");
        t.setPort(1234);
        t.setDescription("Test template");
        t.setParameters("-v");
        t.setEnvironment("PATH=/usr/bin");
        t.setEnabled(true);
        return t;
    }
};

TEST_CASE_METHOD(ProcessTemplateRepoFixture, "Insert and find process template by ID", "[repository][insert][findById]")
{
    ARRANGE(
        ProcessTemplate t = createTestTemplate();
    )

    ACT(
        auto inserted = repository->save(t);
    )

    ASSERT(
        REQUIRE(inserted.has_value());
        auto found = repository->findById(inserted->getId());
        REQUIRE(found.has_value());
        CHECK(found->getName() == t.getName());
        CHECK(found->getCommand() == t.getCommand());
    )
}

TEST_CASE_METHOD(ProcessTemplateRepoFixture, "Find all process templates", "[repository][findAll]")
{
    ARRANGE(
        repository->save(createTestTemplate("T1"));
        repository->save(createTestTemplate("T2"));
    )

    ACT(
        auto templates = repository->findAll();
    )

    ASSERT(
        REQUIRE(templates.size() == 2);
        CHECK(templates[0].getName() == "T1");
        CHECK(templates[1].getName() == "T2");
    )
}

TEST_CASE_METHOD(ProcessTemplateRepoFixture, "Delete existing process template", "[repository][delete]")
{
    ARRANGE(
        auto inserted = repository->save(createTestTemplate());
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

TEST_CASE_METHOD(ProcessTemplateRepoFixture, "Delete non-existent process template returns true", "[repository][delete]")
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

TEST_CASE_METHOD(ProcessTemplateRepoFixture, "Save with ID 0 performs insert", "[repository][save]")
{
    ARRANGE(
        ProcessTemplate t = createTestTemplate();
        t.setId(0);
    )

    ACT(
        auto result = repository->save(t);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() > 0);
        CHECK(result->getName() == t.getName());
    )
}

TEST_CASE_METHOD(ProcessTemplateRepoFixture, "Save with positive ID performs update", "[repository][save]")
{
    ARRANGE(
        auto inserted = repository->save(createTestTemplate("Original"));
        REQUIRE(inserted.has_value());

        ProcessTemplate updated = *inserted;
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
