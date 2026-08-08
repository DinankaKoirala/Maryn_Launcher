cmake_minimum_required(VERSION 3.19)
project(Maryn_Launcher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

# ─── Source Files ─────────────────────────────────────────────────────────────

set(CORE_SOURCES
    # auth
    # src/core/auth/OfflineAuthProvider.cpp

    # instance
    # src/core/instance/Instance.cpp
    # src/core/instance/InstanceManager.cpp

    # download
    src/core/download/VersionManifest.cpp
    src/core/download/Headers/VersionManifest.h
    # src/core/download/DownloadManager.cpp
    # src/core/download/AssetManager.cpp

    # launch
    # src/core/launch/LaunchEngine.cpp

    # java
    # src/core/java/JavaManager.cpp

    # skin
    # src/core/skin/LocalSkinProvider.cpp
)

set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.ui
    # src/ui/InstanceGrid.cpp
    # src/ui/InstanceGrid.h
    # src/ui/AccountPanel.cpp
    # src/ui/AccountPanel.h
    # src/ui/SettingsDialog.cpp
    # src/ui/SettingsDialog.h
)

# ─── Executable ───────────────────────────────────────────────────────────────

qt_add_executable(Maryn_Launcher
    MACOSX_BUNDLE
    main.cpp
    ${CORE_SOURCES}
    ${UI_SOURCES}
)

# ─── Include Dirs ─────────────────────────────────────────────────────────────

target_include_directories(Maryn_Launcher PRIVATE
    src/core
    src/core/download/Headers
    src/ui
)

# ─── Link Libraries ───────────────────────────────────────────────────────────

target_link_libraries(Maryn_Launcher
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

# ─── Install ──────────────────────────────────────────────────────────────────

install(TARGETS Maryn_Launcher
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Maryn_Launcher
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# ─── Windows Deploy ───────────────────────────────────────────────────────────

if(WIN32)
    add_custom_command(TARGET Maryn_Launcher POST_BUILD
        COMMAND C:/Qt/6.11.1/mingw_64/bin/windeployqt.exe
                --no-translations
                --no-system-d3d-compiler
                $<TARGET_FILE:Maryn_Launcher>
        COMMENT "Running windeployqt..."
    )
endif()
