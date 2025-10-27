include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.10.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(DevPilotTests
  repositories/AppRepositoryTest.cpp
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql)

target_link_libraries(DevPilotTests PRIVATE
  Catch2::Catch2WithMain
  Qt${QT_VERSION_MAJOR}::Sql
)
