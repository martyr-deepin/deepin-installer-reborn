CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project(simpleini)

set(SOURCES
    SimpleIni.h
    ConvertUTF.h
    ConvertUTF.c
)

add_library(simpleini STATIC ${SOURCES})
