cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(qtermwidget)

include(GNUInstallDirs)
include(CheckFunctionExists)

option(BUILD_TEST "Build test application. Default OFF." OFF)
# just change version for releases
set(QTERMWIDGET_VERSION_MAJOR "0")
set(QTERMWIDGET_VERSION_MINOR "7")
set(QTERMWIDGET_VERSION_PATCH "1")

set(QTERMWIDGET_VERSION "${QTERMWIDGET_VERSION_MAJOR}.${QTERMWIDGET_VERSION_MINOR}.${QTERMWIDGET_VERSION_PATCH}")

# additional cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include_directories(
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_BINARY_DIR}/lib"
    "${CMAKE_BINARY_DIR}"
)

set(QTERMWIDGET_LIBRARY_NAME qtermwidget5)

option(USE_UTF8PROC "Use libutf8proc for better Unicode support. Default OFF" OFF)

if(USE_UTF8PROC)
    find_package(Utf8Proc)
endif()

if (UTF8PROC_FOUND)
    add_definitions(-DHAVE_UTF8PROC)
    include_directories("${UTF8PROC_INCLUDE_DIRS}")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_definitions("-DBUNDLE_KEYBOARDLAYOUTS")
add_definitions("-DBUNDLE_COLORSCHEMES")

# main library
set(SRCS
    lib/BlockArray.cpp
    lib/ColorScheme.cpp
    lib/Emulation.cpp
    lib/Filter.cpp
    lib/History.cpp
    lib/HistorySearch.cpp
    lib/KeyboardTranslator.cpp
    lib/konsole_wcwidth.cpp
    lib/kprocess.cpp
    lib/kpty.cpp
    lib/kptydevice.cpp
    lib/kptyprocess.cpp
    lib/Pty.cpp
    lib/qtermwidget.cpp
    lib/Screen.cpp
    lib/ScreenWindow.cpp
    lib/SearchBar.cpp
    lib/Session.cpp
    lib/ShellCommand.cpp
    lib/TerminalCharacterDecoder.cpp
    lib/TerminalDisplay.cpp
    lib/tools.cpp
    lib/Vt102Emulation.cpp
)

set(UI_FILES lib/SearchBar.ui)

set(QRC_FILES
    lib/kb-layouts/kb-layouts.qrc
    lib/color-schemes/color-schemes.qrc)

# Only the Headers that need to be moc'd go here
set(HDRS
    lib/Emulation.h
    lib/Filter.h
    lib/HistorySearch.h
    lib/kprocess.h
    lib/kptydevice.h
    lib/kptyprocess.h
    lib/Pty.h
    lib/qtermwidget.h
    lib/ScreenWindow.h
    lib/SearchBar.h
    lib/Session.h
    lib/TerminalDisplay.h
    lib/Vt102Emulation.h
)

# for distribution
set(HDRS_DISTRIB
    lib/qtermwidget.h
    lib/Filter.h
)

#| Defines
add_definitions(-DHAVE_POSIX_OPENPT -DHAVE_SYS_TIME_H)
if(APPLE)
    add_definitions(-DHAVE_UTMPX -D_UTMPX_COMPAT)
endif()
CHECK_FUNCTION_EXISTS(updwtmpx HAVE_UPDWTMPX)
if(HAVE_UPDWTMPX)
    add_definitions(-DHAVE_UPDWTMPX)
endif()

set(Qt_LIBS
    Qt5::Gui
    Qt5::Widgets
    )

add_library(${QTERMWIDGET_LIBRARY_NAME} STATIC
            ${SRCS}
            ${UI_FILES}
            ${HDRS}

            ${QRC_FILES}
            )
target_link_libraries(${QTERMWIDGET_LIBRARY_NAME} ${Qt_LIBS})
set_target_properties(${QTERMWIDGET_LIBRARY_NAME} PROPERTIES
                      SOVERSION ${QTERMWIDGET_VERSION_MAJOR}
                      VERSION ${QTERMWIDGET_VERSION})
if (UTF8PROC_FOUND)
    target_link_libraries(${QTERMWIDGET_LIBRARY_NAME} ${UTF8PROC_LIBRARIES})
endif()

# test application
if(BUILD_TEST)
    set(TEST_SRC src/main.cpp)
    add_executable(test-term
                   ${TEST_SRC}

                   ${QRC_FILES}
                   )
    add_dependencies(test-term ${QTERMWIDGET_LIBRARY_NAME})
    link_directories(${CMAKE_BINARY_DIR})
    target_link_libraries(test-term ${Qt_LIBS} ${QTERMWIDGET_LIBRARY_NAME})
endif (BUILD_TEST)
# end of test application