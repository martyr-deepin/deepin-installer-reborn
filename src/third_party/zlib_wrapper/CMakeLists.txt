CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project(zlib_wrapper)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Core REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(ZLib REQUIRED zlib)
include_directories(AFTER ${ZLib_INCLUDE_DIRS})

set(SOURCES
    zip_util.cpp
    zip_util.h
    )
set(LINK_LIBS
    Qt5::Core
    ${ZLib_LIBRARIES}
    )

add_library(zlib_wrapper STATIC ${SOURCES})
target_link_libraries(zlib_wrapper ${LINK_LIBS})

# TODO(xushaohua): Add unittest