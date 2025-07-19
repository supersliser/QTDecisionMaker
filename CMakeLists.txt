cmake_minimum_required(VERSION 3.16)

project(QTDecisionMaker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages - Qt6 first, then fmt
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Test Sql)
find_package(fmt REQUIRED)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

set(PROJECT_SOURCES
        src/TableViewerWindow.cpp
        include/TableViewerWindow.h
        ui/tableviewerwindow.ui
        src/Column.cpp
        include/Column.h
        src/Row.cpp
        include/Row.h
        src/Table.cpp
        include/Table.h
        src/TableViewerToolbar.cpp
        include/TableViewerToolbar.h
        src/TableViewerMenubar.cpp
        include/TableViewerMenubar.h
        src/MenuBarFile.cpp
        include/MenuBarFile.h
        src/FileSystemManager.cpp
        include/FileSystemManager.h
        include/DataType.h
        include/DataTypes/LinkType.h
        src/DataType.cpp
        include/DataTypes/DescType.h
        include/DataTypes/BoolType.h
        include/DataTypes/RatingType.h
        include/DataTypes/MoneyType.h
        include/DataTypes/NumberType.h
        include/DataTypes/NameType.h
        src/TableManager.cpp
        include/TableManager.h
        src/TableDataDock.cpp
        include/TableDataDock.h
        src/TableColumnDataDock.cpp
        include/TableColumnDataDock.h
        include/MenuBarEdit.h
        src/MenuBarEdit.cpp
        include/MenuBarHelp.h
        src/MenuBarHelp.cpp
)

# Add Windows resource file on Windows
if(WIN32)
    list(APPEND PROJECT_SOURCES app.rc)
endif()
include_directories(include)
    qt_add_executable(QTDecisionMaker
        MANUAL_FINALIZATION
            src/main.cpp
            ${PROJECT_SOURCES}
    )


target_link_libraries(QTDecisionMaker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Sql Qt6::Core Qt6::Gui fmt::fmt)

set_target_properties(QTDecisionMaker PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QTDecisionMaker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QTDecisionMaker)
endif()

qt_standard_project_setup()

qt_add_executable(Tests
        ${PROJECT_SOURCES}
    tests/test_main.cpp
        tests/test_main.h
        tests/columntests.cpp
        tests/rowtests.cpp
        tests/tabletests.cpp

)

set_target_properties(Tests PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(Tests PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Test
    Qt6::Sql
    Qt6::Widgets
    fmt::fmt
)

install(TARGETS Tests
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Deployment scripts disabled for Linux build

# Include Windows-specific deployment configuration
if(WIN32)
    include(toolchain-windows.cmake)
endif()

# CPack configuration for Windows installer
set(CPACK_PACKAGE_NAME "QTDecisionMaker")
set(CPACK_PACKAGE_VENDOR "supersliser")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A decision-making application based on QT")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "QTDecisionMaker")

# Windows-specific installer configuration
if(WIN32)
    # Use NSIS generator for Windows installer
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "QT Decision Maker")
    set(CPACK_NSIS_PACKAGE_NAME "QT Decision Maker")
    set(CPACK_NSIS_CONTACT "https://github.com/supersliser/QTDecisionMaker")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/supersliser/QTDecisionMaker")
    set(CPACK_NSIS_HELP_LINK "https://github.com/supersliser/QTDecisionMaker/discussions")

    # Create desktop shortcut
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$DESKTOP\\\\QT Decision Maker.lnk' '$INSTDIR\\\\bin\\\\QTDecisionMaker.exe'"
    )
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$DESKTOP\\\\QT Decision Maker.lnk'"
    )

    # Add to Programs menu
    set(CPACK_NSIS_MENU_LINKS
        "bin/QTDecisionMaker.exe" "QT Decision Maker"
    )

    # License file
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

    # Enable modify/repair/remove in Add/Remove Programs
    set(CPACK_NSIS_MODIFY_PATH ON)

    # Additional installer options
    set(CPACK_NSIS_COMPRESSOR "/SOLID lzma")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
endif()

# Include CPack
include(CPack)
