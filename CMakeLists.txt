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

pkg_search_module(X11 REQUIRED x11)
pkg_search_module(X11EXT REQUIRED xext)
pkg_search_module(X11TST REQUIRED xtst)

include_directories(AFTER ${X11_INCLUDE_DIRS})
include_directories(AFTER ${X11EXT_INCLUDE_DIRS})
include_directories(AFTER ${X11TST_INCLUDE_DIRS})

set(BASE_FILES
    base/consts.h
    base/file_util.cpp base/file_util.h
    )

set(SERVICE_FILES
    service/signal_manager.cpp service/signal_manager.h
    service/settings_manager.cpp service/settings_manager.h
    )

set(UI_FILES
    ui/main_window.cpp ui/main_window.h
    ui/widgets/flat_button.cpp ui/widgets/flat_button.h
    ui/utils/xutil.cpp ui/utils/xutil.h
    )

set(SOURCE_FILES main.cpp
    application.cpp application.h
    )

set(LINK_LIBS
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::Gui

    ${X11_LIBRARIES}
    ${X11EXT_LIBRARIES}
    ${X11TST_LIBRARIES}
    )

add_executable(deepin-installer-reborn
               ${SOURCE_FILES}

               ${BASE_FILES}
               ${SERVICE_FILES}
               ${UI_FILES}

               resources/resources.qrc
               ui/images/images.qrc
               ui/styles/styles.qrc
               )

target_link_libraries(deepin-installer-reborn ${LINK_LIBS})

install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/deepin-installer-reborn"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(FILES resources/deepin-installer-reborn.desktop
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
install(FILES resources/deepin-installer-reborn.svg
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps")
