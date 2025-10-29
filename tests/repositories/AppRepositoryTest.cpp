// clang-format off

#include "../../src/repositories/AppRepository.h"
#include "../../src/models/App.h"
#include "../helpers/TestHelpers.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <catch2/catch_test_macros.hpp>

struct AppRepoFixture
{
    QSqlDatabase db;
    std::unique_ptr<AppRepository> repository;

    AppRepoFixture()
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "app_test_connection");
        db.setDatabaseName(":memory:");
        REQUIRE(db.open());

        QSqlQuery query(db);
        bool success = query.exec(R"(
            CREATE TABLE IF NOT EXISTS apps (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                path TEXT NOT NULL,
                arguments TEXT,
                enabled BOOLEAN NOT NULL DEFAULT 1,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                UNIQUE(name)
            )
        )");
        REQUIRE(success);

        success = query.exec(R"(
            CREATE TABLE IF NOT EXISTS project_apps (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                project_id INTEGER NOT NULL,
                app_id INTEGER NOT NULL,
                FOREIGN KEY (project_id) REFERENCES projects (id) ON DELETE CASCADE,
                FOREIGN KEY (app_id) REFERENCES apps (id) ON DELETE CASCADE,
                UNIQUE(project_id, app_id)
            )
        )");
        REQUIRE(success);

        repository = std::make_unique<AppRepository>(db);
    }

    ~AppRepoFixture()
    {
        repository.reset();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase("app_test_connection");
    }

    App createTestApp(const QString& name = "TestApp",
                      const QString& path = "/usr/bin/test",
                      const QString& args = "--verbose",
                      bool enabled = true)
    {
        App app;
        app.setName(name);
        app.setPath(path);
        app.setArguments(args);
        app.setEnabled(enabled);
        return app;
    }
};

TEST_CASE_METHOD(AppRepoFixture, "Insert and find app by ID", "[repository][insert][findById]")
{
    ARRANGE(
        App app = createTestApp("CodeEditor", "/usr/bin/code", "--new-window", true);
    )

    ACT(
        auto inserted = repository->save(app);
    )

    ASSERT(
        REQUIRE(inserted.has_value());
        auto found = repository->findById(inserted->getId());
        REQUIRE(found.has_value());
        CHECK(found->getName() == "CodeEditor");
        CHECK(found->getPath() == "/usr/bin/code");
        CHECK(found->getArguments() == "--new-window");
        CHECK(found->isEnabled());
    )
}

TEST_CASE_METHOD(AppRepoFixture, "Find by ID returns nullopt for non-existent app", "[repository][findById]")
{
    ARRANGE(
        int nonExistentId = 1234;
    )

    ACT(
        auto found = repository->findById(nonExistentId);
    )

    ASSERT(
        CHECK_FALSE(found.has_value());
    )
}

TEST_CASE_METHOD(AppRepoFixture, "FindAll returns all apps", "[repository][findAll]")
{
    ARRANGE(
        repository->save(createTestApp("AppOne"));
        repository->save(createTestApp("AppTwo"));
    )

    ACT(
        auto apps = repository->findAll();
    )

    ASSERT(
        REQUIRE(apps.size() == 2);
        CHECK(apps[0].getName() == "AppOne");
        CHECK(apps[1].getName() == "AppTwo");
    )
}

TEST_CASE_METHOD(AppRepoFixture, "Update existing app", "[repository][update]")
{
    ARRANGE(
        auto inserted = repository->save(createTestApp("OldApp", "/old/path", "--quiet", false));
        REQUIRE(inserted.has_value());

        App updated = *inserted;
        updated.setName("UpdatedApp");
        updated.setPath("/new/path");
        updated.setArguments("--debug");
        updated.setEnabled(true);
    )

    ACT(
        auto result = repository->save(updated);
    )

    ASSERT(
        REQUIRE(result.has_value());
        auto found = repository->findById(result->getId());
        REQUIRE(found.has_value());
        CHECK(found->getName() == "UpdatedApp");
        CHECK(found->getPath() == "/new/path");
        CHECK(found->getArguments() == "--debug");
        CHECK(found->isEnabled());
    )
}

TEST_CASE_METHOD(AppRepoFixture, "Delete existing app", "[repository][delete]")
{
    ARRANGE(
        auto inserted = repository->save(createTestApp("ToDelete"));
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

TEST_CASE_METHOD(AppRepoFixture, "Delete non-existent app returns true", "[repository][delete]")
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

TEST_CASE_METHOD(AppRepoFixture, "Multiple inserts generate sequential IDs", "[repository][insert]")
{
    ARRANGE(
        auto app1 = repository->save(createTestApp("App1"));
        auto app2 = repository->save(createTestApp("App2"));
        auto app3 = repository->save(createTestApp("App3"));
    )

    ACT()

    ASSERT(
        REQUIRE(app1.has_value());
        REQUIRE(app2.has_value());
        REQUIRE(app3.has_value());
        CHECK(app2->getId() > app1->getId());
        CHECK(app3->getId() > app2->getId());
    )
}

TEST_CASE_METHOD(AppRepoFixture, "Save with ID 0 performs insert", "[repository][save]")
{
    ARRANGE(
        App app = createTestApp("InsertApp");
        app.setId(0);
    )

    ACT(
        auto result = repository->save(app);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() > 0);
        CHECK(result->getName() == "InsertApp");
    )
}

TEST_CASE_METHOD(AppRepoFixture, "Save with positive ID performs update", "[repository][save]")
{
    ARRANGE(
        auto inserted = repository->save(createTestApp("OriginalApp"));
        REQUIRE(inserted.has_value());

        App updated = *inserted;
        updated.setName("ModifiedApp");
        updated.setEnabled(false);
    )

    ACT(
        auto result = repository->save(updated);
    )

    ASSERT(
        REQUIRE(result.has_value());
        CHECK(result->getId() == inserted->getId());
        CHECK(result->getName() == "ModifiedApp");
        CHECK_FALSE(result->isEnabled());
    )
}

TEST_CASE_METHOD(AppRepoFixture, "setLinkedApps overwrites previous links", "[repository][setLinkedApps]")
{
    ARRANGE(
        auto app1 = repository->save(createTestApp("AppOne"));
        auto app2 = repository->save(createTestApp("AppTwo"));
        auto app3 = repository->save(createTestApp("AppThree"));
        REQUIRE(app1.has_value());
        REQUIRE(app2.has_value());
        REQUIRE(app3.has_value());

        int projectId = 20;
        repository->setLinkedApps(projectId, {app1->getId(), app2->getId()});
    )

    ACT(
        repository->setLinkedApps(20, {app3->getId()});
        auto linkedApps = repository->findByProjectId(20);
    )

    ASSERT(
        REQUIRE(linkedApps.size() == 1);
        CHECK(linkedApps[0].getName() == "AppThree");
    )
}
