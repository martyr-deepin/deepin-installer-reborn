CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

PROJECT(GlobalShortcut)

find_package(Qt5Core REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(X11 REQUIRED x11)
pkg_search_module(X11EXT REQUIRED xext)

include_directories(AFTER ${X11_INCLUDE_DIRS})
include_directories(AFTER ${X11EXT_INCLUDE_DIRS})

SET(LIB_FILES
    global_shortcut.cpp global_shortcut.h
    keyboard_event_filter.cpp keyboard_event_filter.h
    )

set(LINK_LIBS
    Qt5::X11Extras
    ${X11_LIBRARIES}
    ${X11EXT_LIBRARIES}
    )

ADD_LIBRARY(global_shortcut ${LIB_FILES})
target_link_libraries(global_shortcut ${LINK_LIBS})