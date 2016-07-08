cmake_minimum_required(VERSION 3.0)
project(deepin-installer-reborn)

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

set(UI_FILES
    ui/main_window.cpp ui/main_window.h)

set(LINK_LIBS
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::Gui
    )

add_executable(deepin-installer-reborn
               ${SOURCE_FILES}

               ${SERVICE_FILES}
               ${UI_FILES}

               resources/misc.qrc
               )

target_link_libraries(deepin-installer-reborn ${LINK_LIBS})

install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/deepin-installer-reborn"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(FILES resources/misc/deepin-installer-reborn.desktop
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
install(FILES resources/misc/deepin-installer-reborn.svg
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps")
