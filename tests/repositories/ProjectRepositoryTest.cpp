// clang-format off

#include "../../src/repositories/ProjectRepository.h"
#include "../../src/models/Project.h"
#include "../helpers/TestHelpers.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <catch2/catch_test_macros.hpp>

struct ProjectRepoFixture
{
    QSqlDatabase db;
    std::unique_ptr<ProjectRepository> repository;

    ProjectRepoFixture()
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "project_test_connection");
        db.setDatabaseName(":memory:");
        REQUIRE(db.open());

        QSqlQuery query(db);
        bool success = query.exec(R"(
            CREATE TABLE IF NOT EXISTS projects (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                directory_path TEXT NOT NULL,
                description TEXT,
                last_opened_at DATETIME,
                created_at DATETIME NOT NULL,
                updated_at DATETIME NOT NULL,
                UNIQUE(name, directory_path)
            )
        )");
        REQUIRE(success);

        repository = std::make_unique<ProjectRepository>(db);
    }

    ~ProjectRepoFixture()
    {
        repository.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("project_test_connection");
    }

    Project createTestProject(const QString& name = "TestProject",
                              const QString& path = "/tmp/project",
                              const QString& description = "A test project",
                              const QDateTime& lastOpened = QDateTime())
    {
        Project project;
        project.setName(name);
        project.setDirectoryPath(path);
        project.setDescription(description);
        project.setLastOpenedAt(lastOpened);
        return project;
    }
};

TEST_CASE_METHOD(ProjectRepoFixture, "Insert and find project by ID", "[repository][insert][findById]")
{
    ARRANGE(
        Project project = createTestProject("MyProject", "/home/user/myproj", "Description");
    )

    ACT(
        auto inserted = repository->save(project);
    )

    ASSERT(
        REQUIRE(inserted.has_value());
        auto found = repository->findById(inserted->getId());
        REQUIRE(found.has_value());
        CHECK(found->getName() == "MyProject");
        CHECK(found->getDirectoryPath() == "/home/user/myproj");
        CHECK(found->getDescription() == "Description");
    )
}

TEST_CASE_METHOD(ProjectRepoFixture, "Find project by name", "[repository][findByName]")
{
    ARRANGE(
        auto inserted = repository->save(createTestProject("UniqueName"));
        REQUIRE(inserted.has_value());
    )

    ACT(
        auto found = repository->findByName("UniqueName");
    )

    ASSERT(
        REQUIRE(found.has_value());
        CHECK(found->getId() == inserted->getId());
    )
}

TEST_CASE_METHOD(ProjectRepoFixture, "Find by directory path", "[repository][findByDirectoryPath]")
{
    ARRANGE(
        repository->save(createTestProject("Proj1", "/dir/path1"));
        repository->save(createTestProject("Proj2", "/dir/path1"));
    )

    ACT(
        auto projects = repository->findByDirectoryPath("/dir/path1");
    )

    ASSERT(
        REQUIRE(projects.size() == 2);
        CHECK(projects[0].getDirectoryPath() == "/dir/path1");
        CHECK(projects[1].getDirectoryPath() == "/dir/path1");
    )
}

TEST_CASE_METHOD(ProjectRepoFixture, "FindAll returns all projects ordered by name", "[repository][findAll]")
{
    ARRANGE(
        repository->save(createTestProject("Zeta"));
        repository->save(createTestProject("Alpha"));
        repository->save(createTestProject("Beta"));
    )

    ACT(
        auto projects = repository->findAll();
    )

    ASSERT(
        REQUIRE(projects.size() == 3);
        CHECK(projects[0].getName() == "Alpha");
        CHECK(projects[1].getName() == "Beta");
        CHECK(projects[2].getName() == "Zeta");
    )
}

TEST_CASE_METHOD(ProjectRepoFixture, "FindAllByRecentlyOpened returns projects ordered by last_opened_at", "[repository][findAllByRecentlyOpened]")
{
    ARRANGE(
        repository->save(createTestProject("OldProj", "/old", "desc", QDateTime::currentDateTime().addDays(-2)));
        repository->save(createTestProject("NewProj", "/new", "desc", QDateTime::currentDateTime()));
    )

    ACT(
        auto projects = repository->findAllByRecentlyOpened();
    )

    ASSERT(
        REQUIRE(projects.size() == 2);
        CHECK(projects[0].getName() == "NewProj");
        CHECK(projects[1].getName() == "OldProj");
    )
}

TEST_CASE_METHOD(ProjectRepoFixture, "Update last opened timestamp", "[repository][updateLastOpened]")
{
    ARRANGE(
        auto inserted = repository->save(createTestProject("TimeProj"));
        REQUIRE(inserted.has_value());
    )

    ACT(
        bool updated = repository->updateLastOpened(inserted->getId());
    )

    ASSERT(
        REQUIRE(updated);
        auto found = repository->findById(inserted->getId());
        REQUIRE(found.has_value());
        CHECK(found->getLastOpenedAt() <= QDateTime::currentDateTime());
    )
}

TEST_CASE_METHOD(ProjectRepoFixture, "Delete existing project", "[repository][delete]")
{
    ARRANGE(
        auto inserted = repository->save(createTestProject("ToDelete"));
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

TEST_CASE_METHOD(ProjectRepoFixture, "Delete non-existent project returns true", "[repository][delete]")
{
    ARRANGE(
        int nonExistentId = 9999;
    )

    ACT(
        bool result = repository->deleteById(nonExistentId);
    )

    ASSERT(
        CHECK(result);
    )
}

TEST_CASE_METHOD(ProjectRepoFixture, "Save with ID 0 performs insert", "[repository][save]")
{
    ARRANGE(
        Project project = createTestProject("NewInsert");
        project.setId(0);
    )

    ACT(
        auto result = repository->save(project);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() > 0);
        CHECK(result->getName() == "NewInsert");
    )
}

TEST_CASE_METHOD(ProjectRepoFixture, "Save with positive ID performs update", "[repository][save]")
{
    ARRANGE(
        auto inserted = repository->save(createTestProject("OriginalProj"));
        REQUIRE(inserted.has_value());

        Project updated = *inserted;
        updated.setName("ModifiedProj");
    )

    ACT(
        auto result = repository->save(updated);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() == inserted->getId());
        CHECK(result->getName() == "ModifiedProj");
    )
}
