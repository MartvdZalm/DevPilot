
set(PROJECT_SOURCES
    main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DevPilot
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc

        # Components
        components/dialogs/ProjectDialog.h
        components/dialogs/ProjectDialog.cpp
        components/dialogs/ModuleEditDialog.h
        components/dialogs/ModuleEditDialog.cpp
        components/dialogs/NoteDialog.h
        components/dialogs/NoteDialog.cpp
        components/ModuleListItem.h
        components/ModuleListItem.cpp
        components/NoteCard.cpp
        components/NoteCard.h
        components/FlowLayout.h
        components/FlowLayout.cpp
        components/TitleBar.h
        components/TitleBar.cpp
        components/SidebarWidget.cpp
        components/SidebarWidget.h
        components/EmptyStateWidget.cpp
        components/EmptyStateWidget.h
        components/ProjectDetailsWidget.cpp
        components/ProjectDetailsWidget.h
        components/dialogs/ChooseEditorDialog.h
        components/dialogs/ChooseEditorDialog.cpp

        # Database Layer
        database/Database.h
        database/Database.cpp
        database/seeders/ISeeder.h
        database/seeders/Seeder.h
        database/seeders/Seeder.cpp
        database/seeders/ModuleTemplateSeeder.h
        database/seeders/ModuleTemplateSeeder.cpp

        # Events
        events/AppEvents.cpp
        events/AppEvents.h

        # Core
        core/Logger.cpp
        core/Logger.h
        core/ProjectLauncher.cpp
        core/ProjectLauncher.h

        # Models
        models/Model.h
        models/Project.h
        models/Module.h
        models/Note.h
        models/Setting.h
        models/Editor.h
        models/ModuleTemplate.h
        models/App.h

        # Repositories
        repositories/interfaces/IProjectRepository.h
        repositories/interfaces/INoteRepository.h
        repositories/interfaces/IModuleRepository.h
        repositories/interfaces/IEditorRepository.h
        repositories/interfaces/IModuleTemplateRepository.h
        repositories/interfaces/IAppRepository.h
        repositories/ProjectRepository.h
        repositories/ProjectRepository.cpp
        repositories/ModuleRepository.h
        repositories/ModuleRepository.cpp
        repositories/NoteRepository.h
        repositories/NoteRepository.cpp
        repositories/EditorRepository.h
        repositories/EditorRepository.cpp
        repositories/RepositoryProvider.h
        repositories/ModuleTemplateRepository.h
        repositories/ModuleTemplateRepository.cpp
        repositories/AppRepository.h
        repositories/AppRepository.cpp

        # Styles
        styles/AppStyle.h
        styles/AppStyle.cpp
        styles/ButtonStyle.h
        styles/ButtonStyle.cpp
        styles/InputStyle.h
        styles/InputStyle.cpp
        styles/GroupBoxStyle.h
        styles/GroupBoxStyle.cpp
        styles/ListStyle.h
        styles/ListStyle.cpp
        styles/MenuStyle.h
        styles/MenuStyle.cpp
        styles/ModuleStyle.h
        styles/ModuleStyle.cpp
        styles/FontStyle.h
        styles/FontStyle.cpp

        # Windows
        windows/BaseWindow.h
        windows/BaseWindow.cpp
        windows/HomeWindow.h
        windows/HomeWindow.cpp
        windows/MainWindow.h
        windows/MainWindow.cpp
        windows/SettingsWindow.h
        windows/SettingsWindow.cpp
        windows/ModuleWindow.cpp
        windows/ModuleWindow.h
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
