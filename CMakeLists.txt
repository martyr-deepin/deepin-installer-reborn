cmake_minimum_required(VERSION 3.0)
project(deepin_installer)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE MATCHES Release)
  # -Wl, -O1 = Enable linker optimizations
  #-Wl, --gc-sections = Remove unused code resulting from -fdsta-sections and -ffunction-sections
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2  -Wl,-O1 -Wl,--gc-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function")
  # -fno-rtti       = Disable real-time type information
  # -fno-exceptions = Disable exceptions
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -fno-rtti -fno-exceptions")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(PkgConfig REQUIRED)

set(SERVICE_FILES
    service/signal_manager.cpp service/signal_manager.h
    )

set(SOURCE_FILES main.cpp
    application.cpp application.h)

set(LINK_LIBS
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::Gui
    )

add_executable(deepin_installer
               ${SOURCE_FILES}

               ${SERVICE_FILES}
               )

target_link_libraries(deepin_installer ${LINK_LIBS})