include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.10.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(DevPilotTests
  main.cpp
  repositories/SnippetRepositoryTest.cpp
  helpers/TestHelpers.h
  repositories/EditorRepositoryTest.cpp
  repositories/NoteRepositoryTest.cpp
  repositories/AppRepositoryTest.cpp
  repositories/ProjectRepositoryTest.cpp
  repositories/ProcessRepositoryTest.cpp
  repositories/ProcessTemplateRepositoryTest.cpp
)

target_link_libraries(DevPilotTests PRIVATE
  Catch2::Catch2
  DevPilotLib
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Test
)

add_test(NAME DevPilotTests COMMAND DevPilotTests)

set_target_properties(DevPilotTests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
