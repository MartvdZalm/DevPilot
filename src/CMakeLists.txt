
set(PROJECT_SOURCES
    main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DevPilot
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc

        # Components
        components/EmptyStateWidget.cpp
        components/EmptyStateWidget.h
        components/FlowLayout.cpp
        components/FlowLayout.h
        components/NoteCard.cpp
        components/NoteCard.h
        components/ProcessListItem.cpp
        components/ProcessListItem.h
        components/ProjectDetailsWidget.cpp
        components/ProjectDetailsWidget.h
        components/SidebarWidget.cpp
        components/SidebarWidget.h
        components/TitleBar.cpp
        components/TitleBar.h
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
        models/Setting.h
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
        styles/MenuStyle.cpp
        styles/MenuStyle.h
        styles/ModuleStyle.cpp
        styles/ModuleStyle.h

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
else()
    add_executable(DevPilot
        ${PROJECT_SOURCES}
    )
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Network)

target_link_libraries(DevPilot PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Network
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
