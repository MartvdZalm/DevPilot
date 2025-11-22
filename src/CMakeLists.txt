set(PROJECT_SOURCES
    main.cpp
)

add_library(DevPilotLib STATIC
    # Components
    components/home/EmptyStateWidget.cpp
    components/home/EmptyStateWidget.h
    components/shared/FlowLayout.cpp
    components/shared/FlowLayout.h
    components/home/NoteCard.cpp
    components/home/NoteCard.h
    components/home/ProcessListItem.cpp
    components/home/ProcessListItem.h
    components/home/ProjectDetailsWidget.cpp
    components/home/ProjectDetailsWidget.h
    components/home/SidebarWidget.cpp
    components/home/SidebarWidget.h
    components/home/TitleBar.cpp
    components/home/TitleBar.h
    components/dialogs/ChooseEditorDialog.cpp
    components/dialogs/ChooseEditorDialog.h
    components/dialogs/NoteDialog.cpp
    components/dialogs/NoteDialog.h
    components/dialogs/ProcessDialog.cpp
    components/dialogs/ProcessDialog.h
    components/dialogs/ProjectDialog.cpp
    components/dialogs/ProjectDialog.h

    # Core
    core/AnsiHtmlConverter.cpp
    core/AnsiHtmlConverter.h
    core/Logger.cpp
    core/Logger.h
    core/ProjectLauncher.cpp
    core/ProjectLauncher.h
    core/ThemedIcon.h
    core/ThemedIcon.cpp

    # Database Layer
    database/Database.cpp
    database/Database.h
    database/seeders/ISeeder.h
    database/seeders/ProcessTemplateSeeder.cpp
    database/seeders/ProcessTemplateSeeder.h
    database/seeders/Seeder.cpp
    database/seeders/Seeder.h

    # Events
    events/AppEvents.cpp
    events/AppEvents.h

    # Models
    models/App.h
    models/Editor.h
    models/Model.h
    models/Note.h
    models/Process.h
    models/ProcessTemplate.h
    models/Project.h
    models/Snippet.h

    # Repositories
    repositories/AppRepository.cpp
    repositories/AppRepository.h
    repositories/EditorRepository.cpp
    repositories/EditorRepository.h
    repositories/NoteRepository.cpp
    repositories/NoteRepository.h
    repositories/ProcessRepository.cpp
    repositories/ProcessRepository.h
    repositories/ProcessTemplateRepository.cpp
    repositories/ProcessTemplateRepository.h
    repositories/ProjectRepository.cpp
    repositories/ProjectRepository.h
    repositories/RepositoryProvider.h
    repositories/SnippetRepository.cpp
    repositories/SnippetRepository.h
    repositories/interfaces/IAppRepository.h
    repositories/interfaces/IEditorRepository.h
    repositories/interfaces/INoteRepository.h
    repositories/interfaces/IProcessRepository.h
    repositories/interfaces/IProcessTemplateRepository.h
    repositories/interfaces/IProjectRepository.h
    repositories/interfaces/ISnippetRepository.h

    # Styles
    styles/AppStyle.cpp
    styles/AppStyle.h
    styles/ButtonStyle.cpp
    styles/ButtonStyle.h
    styles/FontStyle.cpp
    styles/FontStyle.h
    styles/GroupBoxStyle.cpp
    styles/GroupBoxStyle.h
    styles/InputStyle.cpp
    styles/InputStyle.h
    styles/ListStyle.cpp
    styles/ListStyle.h
    styles/ListItemStyle.cpp
    styles/ListItemStyle.h
    styles/MenuStyle.cpp
    styles/MenuStyle.h
    styles/ModuleStyle.cpp
    styles/ModuleStyle.h
    styles/TableStyle.cpp
    styles/TableStyle.h
    styles/ListItemStyle.cpp
    styles/ListItemStyle.h
    styles/ThemeManager.cpp
    styles/ThemeManager.h
    styles/Theme.h

    # Windows
    windows/BaseWindow.cpp
    windows/BaseWindow.h
    windows/HomeWindow.cpp
    windows/HomeWindow.h
    windows/MainWindow.cpp
    windows/MainWindow.h
    windows/ProcessWindow.cpp
    windows/ProcessWindow.h
    windows/SettingsWindow.cpp
    windows/SettingsWindow.h
    windows/SnippetsWindow.cpp
    windows/SnippetsWindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DevPilot
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
        core/IconManager.cpp
        core/IconManager.h
    )
else()
    add_executable(DevPilot
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(DevPilotLib PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Svg
)

target_include_directories(DevPilotLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(DevPilot PRIVATE
    DevPilotLib
)

set_target_properties(DevPilot PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DevPilot)
endif()

