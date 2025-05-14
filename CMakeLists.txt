cmake_minimum_required(VERSION 3.16)

project(DecisionMaker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Test Sql)
find_package(fmt REQUIRED)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

set(PROJECT_SOURCES
        src/main.cpp
        src/tableviewerwindow.cpp
        include/tableviewerwindow.h
        ui/tableviewerwindow.ui
        src/column.cpp
        include/column.h
        src/row.cpp
        include/row.h
        src/table.cpp
        include/table.h
)
include_directories(include)
    qt_add_executable(DecisionMaker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
            src/tableViewerToolbar.cpp
            include/tableViewerToolbar.h
            src/tableViewerMenubar.cpp
            include/tableViewerMenubar.h
            src/MenuBarFile.cpp
            include/MenuBarFile.h
            src/FileSystemManager.cpp
            include/FileSystemManager.h
    )


target_link_libraries(DecisionMaker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Sql Qt6::Core Qt6::Gui fmt::fmt)

set_target_properties(DecisionMaker PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS DecisionMaker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DecisionMaker)
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
    Qt6::Widgets
)

install(TARGETS Tests
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Tests
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
