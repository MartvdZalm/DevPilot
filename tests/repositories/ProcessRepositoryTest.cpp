// clang-format off

#include "../../src/repositories/ProcessRepository.h"
#include "../../src/models/Process.h"
#include "../helpers/TestHelpers.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <catch2/catch_test_macros.hpp>

struct ProcessRepoFixture
{
    QSqlDatabase db;
    std::unique_ptr<ProcessRepository> repository;

    ProcessRepoFixture()
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "process_test_connection");
        db.setDatabaseName(":memory:");
        REQUIRE(db.open());

        QSqlQuery query(db);
        bool success = query.exec(R"(
            CREATE TABLE IF NOT EXISTS processes (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                project_id INTEGER NOT NULL,
                name TEXT NOT NULL,
                command TEXT NOT NULL,
                working_directory TEXT NOT NULL,
                status TEXT NOT NULL DEFAULT 'stopped',
                pid INTEGER,
                port INTEGER,
                log_path TEXT,
                last_started_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                uptime DATETIME,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY (project_id) REFERENCES projects (id) ON DELETE CASCADE
            )
        )");
        REQUIRE(success);

        repository = std::make_unique<ProcessRepository>(db);
    }

    ~ProcessRepoFixture()
    {
        repository.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("process_test_connection");
    }

    Process createTestProcess(int projectId = 1, const QString& name = "TestProcess")
    {
        Process p;
        p.setProjectId(projectId);
        p.setName(name);
        p.setCommand("echo test");
        p.setWorkingDirectory("/tmp");
        p.setStatus(Process::Status::Stopped);
        p.setPID(0);
        p.setPort(8080);
        p.setLogPath("/tmp/log.txt");
        p.setLastStartedAt(QDateTime());
        p.setUptime(QDateTime());
        return p;
    }
};

TEST_CASE_METHOD(ProcessRepoFixture, "Insert and find process by ID", "[repository][insert][findById]")
{
    ARRANGE(
        Process process = createTestProcess();
    )

    ACT(
        auto inserted = repository->save(process);
    )

    ASSERT(
        REQUIRE(inserted.has_value());
        auto found = repository->findById(inserted->getId());
        REQUIRE(found.has_value());
        CHECK(found->getName() == process.getName());
        CHECK(found->getProjectId() == process.getProjectId());
    )
}

TEST_CASE_METHOD(ProcessRepoFixture, "Find processes by project ID", "[repository][findByProjectId]")
{
    ARRANGE(
        repository->save(createTestProcess(1, "Proc1"));
        repository->save(createTestProcess(1, "Proc2"));
        repository->save(createTestProcess(2, "Proc3"));
    )

    ACT(
        auto procs = repository->findByProjectId(1);
    )

    ASSERT(
        REQUIRE(procs.size() == 2);
        CHECK(procs[0].getProjectId() == 1);
        CHECK(procs[1].getProjectId() == 1);
    )
}

TEST_CASE_METHOD(ProcessRepoFixture, "Find all processes", "[repository][findAll]")
{
    ARRANGE(
        repository->save(createTestProcess(1, "A"));
        repository->save(createTestProcess(2, "B"));
    )

    ACT(
        auto procs = repository->findAll();
    )

    ASSERT(
        REQUIRE(procs.size() == 2);
        CHECK(procs[0].getName() == "A");
        CHECK(procs[1].getName() == "B");
    )
}

TEST_CASE_METHOD(ProcessRepoFixture, "Delete existing process", "[repository][delete]")
{
    ARRANGE(
        auto inserted = repository->save(createTestProcess());
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

TEST_CASE_METHOD(ProcessRepoFixture, "Delete non-existent process returns true", "[repository][delete]")
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

TEST_CASE_METHOD(ProcessRepoFixture, "Save with ID 0 performs insert", "[repository][save]")
{
    ARRANGE(
        Process process = createTestProcess();
        process.setId(0);
    )

    ACT(
        auto result = repository->save(process);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() > 0);
        CHECK(result->getName() == process.getName());
    )
}

TEST_CASE_METHOD(ProcessRepoFixture, "Save with positive ID performs update", "[repository][save]")
{
    ARRANGE(
        auto inserted = repository->save(createTestProcess(1, "Original"));
        REQUIRE(inserted.has_value());

        Process updated = *inserted;
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
