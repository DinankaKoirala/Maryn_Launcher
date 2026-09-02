cmake_minimum_required(VERSION 3.19)
project(Maryn_Launcher VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

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
    src/core/launch/Headers
)

# ─── Bundle Properties ────────────────────────────────────────────────────────

set_target_properties(Maryn_Launcher PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.maryn.launcher"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_COPYRIGHT "Maryn Launcher"
)

# Linux RPATH: allow bundled Qt libs in lib/ next to binary (for AppDir / qt_deploy)
if(LINUX)
    set_target_properties(Maryn_Launcher PROPERTIES
        INSTALL_RPATH "$ORIGIN/lib:$ORIGIN/../lib:$ORIGIN"
        BUILD_RPATH "$ORIGIN/lib:$ORIGIN/../lib:$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH_USE_LINK_PATH TRUE
    )
endif()

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

# ─── Deploy (POST_BUILD) ──────────────────────────────────────────────────────
# install(SCRIPT deploy_script) handles `cmake --install` / CPack.
# POST_BUILD commands make plain `cmake --build` produce a runnable bundle
# on both platforms without requiring `cmake --install`.

if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt
        HINTS "${Qt6_DIR}/../../../bin" "${QT6_INSTALL_PREFIX}/bin"
        PATHS "C:/Qt/6.11.1/mingw_64/bin" "C:/Qt/6.8.0/msvc2022_64/bin" "C:/Qt/Tools/QtCreator/bin"
    )
    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET Maryn_Launcher POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --no-translations
                    --no-system-d3d-compiler
                    --no-opengl-sw
                    "$<TARGET_FILE:Maryn_Launcher>"
            COMMENT "Running windeployqt..."
            VERBATIM
        )
    else()
        message(WARNING "windeployqt not found - Windows bundle will not be self-contained. Set Qt bin dir in PATH.")
    endif()
endif()

if(APPLE)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt
        HINTS "${Qt6_DIR}/../../../bin" "${QT6_INSTALL_PREFIX}/bin"
    )
    if(NOT MACDEPLOYQT_EXECUTABLE AND DEFINED QT6_INSTALL_PREFIX)
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt PATHS "${QT6_INSTALL_PREFIX}/bin" NO_DEFAULT_PATH)
    endif()
    if(NOT MACDEPLOYQT_EXECUTABLE)
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt)
    endif()

    if(MACDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET Maryn_Launcher POST_BUILD
            COMMAND "${MACDEPLOYQT_EXECUTABLE}"
                    "$<TARGET_BUNDLE_DIR:Maryn_Launcher>"
                    -verbose=0
            COMMENT "Running macdeployqt..."
            VERBATIM
        )
        find_program(CODESIGN_EXECUTABLE codesign)
        if(CODESIGN_EXECUTABLE)
            add_custom_command(TARGET Maryn_Launcher POST_BUILD
                COMMAND "${CODESIGN_EXECUTABLE}" --force --deep --sign - "$<TARGET_BUNDLE_DIR:Maryn_Launcher>"
                COMMENT "Ad-hoc codesigning app bundle..."
                VERBATIM
            )
        endif()
    else()
        message(WARNING "macdeployqt not found - macOS bundle will not be self-contained (rpath will point to Homebrew Qt).")
    endif()
endif()

if(LINUX)
    # Prefer linuxdeployqt / linuxdeploy if available (most complete for AppImage/standalone)
    find_program(LINUXDEPLOYQT_EXECUTABLE linuxdeployqt)
    find_program(LINUXDEPLOY_EXECUTABLE linuxdeploy)

    if(LINUXDEPLOYQT_EXECUTABLE)
        # linuxdeployqt expects an AppDir structure
        find_program(QMAKE_EXECUTABLE qmake HINTS "${Qt6_DIR}/../../../bin" "${QT6_INSTALL_PREFIX}/bin")
        if(NOT QMAKE_EXECUTABLE)
            find_program(QMAKE_EXECUTABLE qmake6)
        endif()
        if(QMAKE_EXECUTABLE)
            set(LINUXDEPLOYQT_QMAKE_ARG "-qmake=${QMAKE_EXECUTABLE}")
        else()
            set(LINUXDEPLOYQT_QMAKE_ARG "")
        endif()
        add_custom_command(TARGET Maryn_Launcher POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/AppDir/usr/bin"
            COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:Maryn_Launcher>" "${CMAKE_BINARY_DIR}/AppDir/usr/bin/"
            COMMAND "${LINUXDEPLOYQT_EXECUTABLE}" "${CMAKE_BINARY_DIR}/AppDir/usr/bin/Maryn_Launcher" ${LINUXDEPLOYQT_QMAKE_ARG} -bundle-non-qt-libs -verbose=1
            COMMENT "Running linuxdeployqt..."
            VERBATIM
        )
    elseif(LINUXDEPLOY_EXECUTABLE)
        find_program(LINUXDEPLOY_QT_PLUGIN linuxdeploy-plugin-qt)
        if(LINUXDEPLOY_QT_PLUGIN)
            add_custom_command(TARGET Maryn_Launcher POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/AppDir"
                COMMAND "${LINUXDEPLOY_EXECUTABLE}" --appdir "${CMAKE_BINARY_DIR}/AppDir" --executable "$<TARGET_FILE:Maryn_Launcher>" --plugin qt --output appimage
                COMMENT "Running linuxdeploy with qt plugin (AppImage)..."
                VERBATIM
            )
        else()
            add_custom_command(TARGET Maryn_Launcher POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/AppDir"
                COMMAND "${LINUXDEPLOY_EXECUTABLE}" --appdir "${CMAKE_BINARY_DIR}/AppDir" --executable "$<TARGET_FILE:Maryn_Launcher>" --output appimage
                COMMENT "Running linuxdeploy (no qt plugin)..."
                VERBATIM
            )
        endif()
    else()
        # Fallback: use Qt's own deploy script (handles Qt libs + plugins + qt.conf via ldd/patchelf)
        # Generates a self-contained folder at ${CMAKE_BINARY_DIR}/qt_deploy for distribution
        # `cmake --install` already uses this script; this makes `cmake --build` also bundle.
        find_program(PATCHELF_EXECUTABLE patchelf)
        if(PATCHELF_EXECUTABLE)
            set(_LINUX_PATCHELF_HINT " (patchelf found: rpath will be fixed)")
        else()
            set(_LINUX_PATCHELF_HINT " (patchelf NOT found: install patchelf for rpath fix, fallback uses wrapper script)")
        endif()
        add_custom_command(TARGET Maryn_Launcher POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "No linuxdeployqt/linuxdeploy found${_LINUX_PATCHELF_HINT} - using Qt deploy script fallback..."
            # Run Qt deploy script with QT_DEPLOY_PREFIX pointing to build/qt_deploy
            COMMAND ${CMAKE_COMMAND} -DQT_DEPLOY_PREFIX=${CMAKE_BINARY_DIR}/qt_deploy -P "${deploy_script}"
            COMMENT "Running Qt deploy script for Linux..."
            VERBATIM
        )
        # Wrapper script for running from build tree without full deploy (LD_LIBRARY_PATH fallback)
        # Created at configure time (no POST_BUILD shell redirection needed)
        file(WRITE "${CMAKE_BINARY_DIR}/Maryn_Launcher.sh" [=[#!/bin/bash
# Auto-generated wrapper for bundled Qt libs (Linux)
DIR="$(cd "$(dirname "$0")" && pwd)"
export LD_LIBRARY_PATH="$DIR/qt_deploy/lib:$DIR/lib:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="$DIR/qt_deploy/plugins:$DIR/qt_deploy/lib/qt/plugins:$QT_PLUGIN_PATH"
exec "$DIR/Maryn_Launcher" "$@"
]=])
        execute_process(COMMAND chmod +x "${CMAKE_BINARY_DIR}/Maryn_Launcher.sh")
    endif()
endif()
