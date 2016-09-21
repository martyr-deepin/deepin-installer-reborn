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

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wextra")
  # -fno-rtti       = Disable real-time type information
  # -fno-exceptions = Disable exceptions
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra -fno-rtti -fno-exceptions -fsanitize=address")
else()
  # -Wl, -O1 = Enable linker optimizations
  #-Wl, --gc-sections = Remove unused code resulting from -fdsta-sections and -ffunction-sections
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2  -Wl,-O1 -Wl,--gc-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections -Wno-unused-result")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions("-DBUILTIN_HOOKS_DIR=\"${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn/hooks\"")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(Parted REQUIRED libparted)
pkg_search_module(QREncode REQUIRED libqrencode)
pkg_search_module(X11 REQUIRED x11)
pkg_search_module(X11EXT REQUIRED xext)
pkg_search_module(X11TST REQUIRED xtst)

include_directories(AFTER ${Parted_INCLUDE_DIRS})
include_directories(AFTER ${QREncode_INCLUDE_DIRS})
include_directories(AFTER ${X11_INCLUDE_DIRS})
include_directories(AFTER ${X11EXT_INCLUDE_DIRS})
include_directories(AFTER ${X11TST_INCLUDE_DIRS})

add_subdirectory(third_party/CuteLogger)
include_directories(third_party/CuteLogger)
add_subdirectory(third_party/googletest)
include_directories(third_party/googletest)

set(BASE_FILES
    base/command.cpp base/command.h
    base/consts.h
    base/file_util.cpp base/file_util.h
    base/string_util.cpp base/string_util.h
    )

set(OEM_FILES
    misc/oem_tool.cpp
    misc/oem/oem_window.cpp misc/oem/oem_window.h
    misc/oem/section_label.cpp misc/oem/section_label.h
    )

set(PARTMAN_FILES
    partman/device.cpp partman/device.h
    partman/fs.cpp partman/fs.h
    partman/operation.cpp partman/operation.h
    partman/operation_create.cpp partman/operation_create.h
    partman/operation_delete.cpp partman/operation_delete.h
    partman/operation_format.cpp partman/operation_format.h
    partman/operation_mount_point.cpp partman/operation_mount_point.h
    partman/operation_resize.cpp partman/operation_resize.h
    partman/partition.cpp partman/partition.h
    partman/partition_format.cpp partman/partition_format.h
    partman/partition_label.cpp partman/partition_label.h
    partman/partition_manager.cpp partman/partition_manager.h
    partman/partition_usage.cpp partman/partition_usage.h
    partman/structs.h
    partman/utils.cpp partman/utils.h
    )

set(SERVICE_FILES
    service/hooks_manager.cpp service/hooks_manager.h
    service/log_manager.cpp service/log_manager.h
    service/settings_manager.cpp service/settings_manager.h
    service/signal_manager.cpp service/signal_manager.h
    service/system_language.cpp service/system_language.h
    service/settings_name.h
    )

set(SYSINFO_FILES
    sysinfo/dev_uuid.cpp sysinfo/dev_uuid.h
    sysinfo/machine.cpp sysinfo/machine.h
    sysinfo/proc_mounts.cpp sysinfo/proc_mounts.h
    sysinfo/proc_partitions.cpp sysinfo/proc_partitions.h
    sysinfo/proc_swaps.cpp sysinfo/proc_swaps.h
    sysinfo/timezone.cpp sysinfo/timezone.h
    sysinfo/users.cpp sysinfo/users.h
    sysinfo/validate_hostname.cpp sysinfo/validate_hostname.h
    sysinfo/validate_password.cpp sysinfo/validate_password.h
    sysinfo/validate_username.cpp sysinfo/validate_username.h
    sysinfo/virtual_machine.cpp sysinfo/virtual_machine.h
    )

set(UI_FILES
    ui/main_window.cpp ui/main_window.h
    
    ui/delegates/language_list_delegate.cpp ui/delegates/language_list_delegate.h
       ui/delegates/partition_delegate.cpp ui/delegates/partition_delegate.h
    ui/delegates/partition_util.cpp ui/delegates/partition_util.h

    ui/models/fs_model.cpp ui/models/fs_model.h
    ui/models/language_list_model.cpp ui/models/language_list_model.h
    ui/models/mount_point_model.cpp ui/models/mount_point_model.h

    ui/frames/consts.h
    ui/frames/inner/advanced_partition_frame.cpp ui/frames/inner/advanced_partition_frame.h
    ui/frames/inner/edit_partition_frame.cpp ui/frames/inner/edit_partition_frame.h
    ui/frames/inner/new_partition_frame.cpp ui/frames/inner/new_partition_frame.h
    ui/frames/inner/partition_loading_frame.cpp ui/frames/inner/partition_loading_frame.h
    ui/frames/inner/prepare_install_frame.cpp ui/frames/inner/prepare_install_frame.h
    ui/frames/inner/select_bootloader_frame.cpp ui/frames/inner/select_bootloader_frame.h
    ui/frames/inner/simple_partition_frame.cpp ui/frames/inner/simple_partition_frame.h
    ui/frames/inner/system_info_avatar_frame.cpp ui/frames/inner/system_info_avatar_frame.h
    ui/frames/inner/system_info_form_frame.cpp ui/frames/inner/system_info_form_frame.h
    ui/frames/inner/system_info_timezone_frame.cpp ui/frames/inner/system_info_timezone_frame.h

    ui/frames/confirm_quit_frame.cpp ui/frames/confirm_quit_frame.h
    ui/frames/disk_space_insufficient_frame.cpp ui/frames/disk_space_insufficient_frame.h
    ui/frames/install_failed_frame.cpp ui/frames/install_failed_frame.h
    ui/frames/install_progress_frame.cpp ui/frames/install_progress_frame.h
    ui/frames/install_success_frame.cpp ui/frames/install_success_frame.h
    ui/frames/log_viewer_frame.cpp ui/frames/log_viewer_frame.h
    ui/frames/partition_frame.cpp ui/frames/partition_frame.h
    ui/frames/partition_table_warning_frame.cpp ui/frames/partition_table_warning_frame.h
    ui/frames/select_language_frame.cpp ui/frames/select_language_frame.h
    ui/frames/system_info_frame.cpp ui/frames/system_info_frame.h
    ui/frames/virtual_machine_frame.cpp ui/frames/virtual_machine_frame.h

    ui/widgets/advanced_partition_item.cpp ui/widgets/advanced_partition_item.h
    ui/widgets/avatar_button.cpp ui/widgets/avatar_button.h
    ui/widgets/comment_label.cpp ui/widgets/comment_label.h
    ui/widgets/expanded_nav_button.cpp ui/widgets/expanded_nav_button.h
    ui/widgets/flat_button.cpp ui/widgets/flat_button.h
    ui/widgets/frosted_frame.cpp ui/widgets/frosted_frame.h
    ui/widgets/icon_button.cpp ui/widgets/icon_button.h
    ui/widgets/line_edit.cpp ui/widgets/line_edit.h
    ui/widgets/list_label.cpp ui/widgets/list_label.h
    ui/widgets/list_title_label.cpp ui/widgets/list_title_label.h
    ui/widgets/nav_button.cpp ui/widgets/nav_button.h
    ui/widgets/page_indicator.cpp ui/widgets/page_indicator.h
    ui/widgets/partition_usage_bar.cpp ui/widgets/partition_usage_bar.h
    ui/widgets/qr_widget.cpp ui/widgets/qr_widget.h
    ui/widgets/simple_partition_button.cpp ui/widgets/simple_partition_button.h
    ui/widgets/subhead_label.cpp ui/widgets/subhead_label.h
    ui/widgets/table_combo_box.cpp ui/widgets/table_combo_box.h
    ui/widgets/table_item_label.cpp ui/widgets/table_item_label.h
    ui/widgets/title_label.cpp ui/widgets/title_label.h

    ui/utils/xutil.cpp ui/utils/xutil.h
    )

set(UNITTEST_FILES
    sysinfo/dev_uuid_test.cpp
    sysinfo/proc_mounts_test.cpp
    sysinfo/proc_partitions_test.cpp
    sysinfo/proc_swaps_test.cpp
    sysinfo/timezone_test.cpp
    sysinfo/users_test.cpp
    sysinfo/validate_hostname_test.cpp
    sysinfo/validate_password_test.cpp
    sysinfo/validate_username_test.cpp
    )

set(Qt_LIBS
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::Gui
    )

set(LINK_LIBS
    ${Qt_LIBS}
    ${Parted_LIBRARIES}
    ${QREncode_LIBRARIES}
    ${X11_LIBRARIES}
    ${X11EXT_LIBRARIES}
    ${X11TST_LIBRARIES}

    CuteLogger
    )

# Main executable
add_executable(deepin-installer-reborn
               main.cpp

               ${BASE_FILES}
               ${PARTMAN_FILES}
               ${SERVICE_FILES}
               ${SYSINFO_FILES}
               ${UI_FILES}

               resources/resources.qrc
               ui/images/images.qrc
               ui/styles/styles.qrc
               )

target_link_libraries(deepin-installer-reborn ${LINK_LIBS})

# Unittest
add_executable(deepin-installer-reborn-tests
               misc/test_main.cpp

               ${BASE_FILES}
               ${SERVICE_FILES}
               ${SYSINFO_FILES}
               ${UNITTEST_FILES}

               resources/resources.qrc
               )
target_link_libraries(deepin-installer-reborn-tests
                      ${LINK_LIBS}
                      gtest
                      )


# deepin-installer-oem-tool
add_executable(deepin-installer-oem-tool
               ${OEM_FILES}
               ${BASE_FILES}
               ${SERVICE_FILES}
               ${SYSINFO_FILES}

               misc/oem/styles/oem_styles.qrc
               resources/resources.qrc
               )
target_link_libraries(deepin-installer-oem-tool ${LINK_LIBS})

# unsquasfs_progress
add_executable(unsquashfs-progress-reborn
               misc/unsquashfs_progress.cpp
               
               ${BASE_FILES}
               )
target_link_libraries(unsquashfs-progress-reborn ${Qt_LIBS})


## Installation
install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/deepin-installer-reborn"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/deepin-installer-oem-tool"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/unsquashfs-progress-reborn"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(FILES resources/deepin-installer-reborn.desktop
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
install(FILES resources/deepin-installer-reborn.svg
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps")
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/hooks
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn
    FILES_MATCHING PATTERN "*"
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
