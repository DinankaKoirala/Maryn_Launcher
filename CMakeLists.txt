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
    src/core/instance/InstanceManager.cpp
    src/core/instance/Headers/InstanceManager.h

    # download
    src/core/download/VersionManifest.cpp
    src/core/download/Headers/VersionManifest.h
    src/core/download/Headers/VersionJsonParser.h
    src/core/download/Headers/VersionDetails.h
    src/core/download/VersionJsonParser.cpp
    src/core/download/DownloadManager.cpp
    src/core/download/Headers/DownloadManager.h
    src/core/download/Headers/JavaManager.h
    src/core/download/JavaManager.cpp
    src/core/download/AssetManager.cpp
    src/core/download/Headers/AssetManager.h

    # launch
    src/core/launch/LaunchEngine.cpp
    src/core/launch/Headers/LaunchEngine.h
)

set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.ui
    src/ui/InstanceWindow.cpp
    src/ui/InstanceWindow.h
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
    src/core/launch/Headers
)

# ─── Link Libraries ───────────────────────────────────────────────────────────

target_link_libraries(Maryn_Launcher
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

if(APPLE)
    target_link_libraries(Maryn_Launcher PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework OpenGL"
    )
    target_link_options(Maryn_Launcher PRIVATE
        "-Wl,-undefined,dynamic_lookup"
    )
endif()

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
