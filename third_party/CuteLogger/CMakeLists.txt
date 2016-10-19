CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project(CuteLogger)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Core REQUIRED)

add_definitions(-DCUTELOGGER_LIBRARY)

set(SOURCES
    CuteLogger/AbstractAppender.cpp CuteLogger/AbstractAppender.h
    CuteLogger/AbstractStringAppender.cpp CuteLogger/AbstractStringAppender.h
    CuteLogger/ConsoleAppender.cpp CuteLogger/ConsoleAppender.h
    CuteLogger/CuteLogger_global.h
    CuteLogger/FileAppender.cpp CuteLogger/FileAppender.h
    CuteLogger/Logger.cpp CuteLogger/Logger.h
    CuteLogger/RollingFileAppender.cpp CuteLogger/RollingFileAppender.h
    )

add_library(CuteLogger STATIC ${SOURCES})
target_link_libraries(CuteLogger Qt5::Core)
