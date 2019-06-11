
#set(CMAKE_C_COMPILER  /usr/bin/clang)
#set(CMAKE_CXX_COMPILER  /usr/bin/clang++)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
#   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Werror -Wextra")
  # -fno-rtti, Disable real-time type information
  # -fno-exceptions Disable exceptions
  # -fsanitize=address Enable memory sanitize plugin
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra -fno-exceptions")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-implicit-fallthrough")
  # -fsanitize=address, Enable memory address sanitizer.
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  # -fsanitize=leak, Enable memory leak sanitizer.
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  # -fsanitize=memory, Enable detecting uninitited memory sanitizer.
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory "
  #    "-fsanitize-memory-track-origins -fPIC -pie -fno-omit-frame-pointer")

#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wimplicit-fallthrough=1")

  # Enable Qt builtin debug mode
  add_definitions("-DQT_MESSAGELOGCONTEXT")
else()
  # -Wl, -O2 Enable linker optimizations
  #-Wl, --gc-sections Remove unused code resulting from -fdsta-sections and -ffunction-sections
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -Wl,-O1 -Wl,--gc-sections -Wno-unused-result")
endif()

# Install default_settings.ini based on current architecture.
set(INSTALLER_BUILD_ARCH "")
execute_process(COMMAND uname -m
                OUTPUT_VARIABLE INSTALLER_BUILD_ARCH)
message("build arch: ${INSTALLER_BUILD_ARCH}")

set(IS_PROFESSIONAL FALSE)
if (EXISTS "/etc/deepin-version")
  file(STRINGS "/etc/deepin-version" IS_PROFESSIONAL REGEX "Type *= *Professional")
  if(IS_PROFESSIONAL)
      add_definitions("-DPROFESSIONAL")
  endif()
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(INSTALLER_SHARE_DIR ${CMAKE_INSTALL_PREFIX}/share/deepin-installer)

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

# Defines path to resource directories
if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions("-DBUILTIN_HOOKS_DIR=\"${CMAKE_SOURCE_DIR}/hooks\"")
  add_definitions("-DBUILTIN_CHECK_HOOKS_DIR=\"${CMAKE_SOURCE_DIR}/check_hooks\"")
  add_definitions("-DI18N_DIR=\"${CMAKE_BINARY_DIR}/i18n\"")
  add_definitions("-DRESOURCES_DIR=\"${CMAKE_SOURCE_DIR}/resources\"")
  add_definitions("-DTOOLS_DIR=\"${CMAKE_SOURCE_DIR}/tools\"")
else()
  add_definitions("-DN_DEBUG")
  add_definitions("-DBUILTIN_HOOKS_DIR=\"${INSTALLER_SHARE_DIR}/hooks\"")
  add_definitions("-DBUILTIN_CHECK_HOOKS_DIR=\"${INSTALLER_SHARE_DIR}/check_hooks\"")
  add_definitions("-DI18N_DIR=\"${INSTALLER_SHARE_DIR}/i18n\"")
  add_definitions("-DRESOURCES_DIR=\"${INSTALLER_SHARE_DIR}/resources\"")
  add_definitions("-DTOOLS_DIR=\"${INSTALLER_SHARE_DIR}/tools\"")
endif()

find_package(PkgConfig REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5LinguistTools)
find_package(Qt5Svg REQUIRED)
find_package(Qt5DBus REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

pkg_search_module(Parted REQUIRED libparted)
pkg_search_module(X11 REQUIRED x11)
pkg_search_module(X11EXT REQUIRED xext)
pkg_search_module(X11TST REQUIRED xtst)
pkg_search_module(X11RandR REQUIRED xrandr)

include_directories(AFTER ${Parted_INCLUDE_DIRS})
include_directories(AFTER ${X11_INCLUDE_DIRS})
include_directories(AFTER ${X11EXT_INCLUDE_DIRS})
include_directories(AFTER ${X11TST_INCLUDE_DIRS})
include_directories(AFTER ${X11RandR_INCLUDE_DIRS})

add_subdirectory(third_party/global_shortcut)
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/qrencode)
add_subdirectory(third_party/qtermwidget)
add_subdirectory(third_party/simpleini)
add_subdirectory(third_party/timezones)
add_subdirectory(third_party/zlib_wrapper)

configure_file(base/consts.h base/consts.h @ONLY)

set(BASE_FILES
    base/command.cpp
    base/command.h
    base/consts.h
    base/file_util.cpp
    base/file_util.h
    base/string_util.cpp
    base/string_util.h
    base/thread_util.cpp
    base/thread_util.h
    )

set(OEM_FILES

    oem/models/oem_settings_item.cpp
    oem/models/oem_settings_item.h
    oem/models/oem_settings_model.cpp
    oem/models/oem_settings_model.h
    oem/views/oem_settings_item_view.cpp
    oem/views/oem_settings_item_view.h

    oem/styles/styles.qrc
    )

set(PARTMAN_FILES
    partman/device.cpp
    partman/device.h
    partman/fs.cpp
    partman/fs.h
    partman/libparted_util.cpp
    partman/libparted_util.h
    partman/operation.cpp
    partman/operation.h
    partman/os_prober.cpp
    partman/os_prober.h
    partman/partition.cpp
    partman/partition.h
    partman/partition_format.cpp
    partman/partition_format.h
    partman/partition_manager.cpp
    partman/partition_manager.h
    partman/partition_usage.cpp
    partman/partition_usage.h
    partman/structs.cpp
    partman/structs.h
    partman/utils.cpp
    partman/utils.h
    )

set(SERVICE_FILES
    service/backend/chroot.cpp
    service/backend/chroot.h
    service/backend/geoip_request_worker.cpp
    service/backend/geoip_request_worker.h
    service/backend/hooks_pack.cpp
    service/backend/hooks_pack.h
    service/backend/hook_worker.cpp
    service/backend/hook_worker.h
    service/backend/wifi_inspect_worker.cpp
    service/backend/wifi_inspect_worker.h

    service/first_boot_hook_worker.cpp
    service/first_boot_hook_worker.h
    service/hooks_manager.cpp
    service/hooks_manager.h
    service/log_manager.cpp
    service/log_manager.h
    service/power_manager.cpp
    service/power_manager.h
    service/process_util.cpp
    service/process_util.h
    service/screen_brightness.cpp
    service/screen_brightness.h
    service/settings_manager.cpp
    service/settings_manager.h
    service/system_language.cpp
    service/system_language.h
    service/settings_name.h
    service/timezone_manager.cpp
    service/timezone_manager.h
    )

set(SYSINFO_FILES
    sysinfo/dev_disk.cpp
    sysinfo/dev_disk.h
    sysinfo/iso3166.cpp
    sysinfo/iso3166.h
    sysinfo/keyboard.cpp
    sysinfo/keyboard.h
    sysinfo/machine.cpp
    sysinfo/machine.h
    sysinfo/proc_meminfo.cpp
    sysinfo/proc_meminfo.h
    sysinfo/proc_mounts.cpp
    sysinfo/proc_mounts.h
    sysinfo/proc_partitions.cpp
    sysinfo/proc_partitions.h
    sysinfo/proc_swaps.cpp
    sysinfo/proc_swaps.h
    sysinfo/release_version.cpp
    sysinfo/release_version.h
    sysinfo/timezone.cpp
    sysinfo/timezone.h
    sysinfo/users.cpp
    sysinfo/users.h
    sysinfo/validate_hostname.cpp
    sysinfo/validate_hostname.h
    sysinfo/validate_password.cpp
    sysinfo/validate_password.h
    sysinfo/validate_username.cpp
    sysinfo/validate_username.h
    sysinfo/virtual_machine.cpp
    sysinfo/virtual_machine.h)

set(UI_FILES

    ui/delegates/advanced_partition_animations.cpp
    ui/delegates/advanced_partition_animations.h
    ui/delegates/advanced_partition_delegate.cpp
    ui/delegates/advanced_partition_delegate.h
    ui/delegates/advanced_validate_state.h
    ui/delegates/avatar_list_delegate.cpp
    ui/delegates/avatar_list_delegate.h
    ui/delegates/frameless_list_delegate.cpp
    ui/delegates/frameless_list_delegate.h
    ui/delegates/full_disk_delegate.cpp
    ui/delegates/full_disk_delegate.h
    ui/delegates/installer_args_parser.cpp
    ui/delegates/installer_args_parser.h
    ui/delegates/install_slide_frame_util.cpp
    ui/delegates/install_slide_frame_util.h
    ui/delegates/language_delegate.cpp
    ui/delegates/language_delegate.h
    ui/delegates/main_window_util.cpp
    ui/delegates/main_window_util.h
    ui/delegates/partition_util.cpp
    ui/delegates/partition_util.h
    ui/delegates/popup_menu_delegate.cpp
    ui/delegates/popup_menu_delegate.h
    ui/delegates/simple_partition_delegate.cpp
    ui/delegates/simple_partition_delegate.h
    ui/delegates/table_combo_box_delegate.cpp
    ui/delegates/table_combo_box_delegate.h
    ui/delegates/timezone_map_util.cpp
    ui/delegates/timezone_map_util.h

    ui/frames/consts.h
    ui/frames/inner/advanced_partition_frame.cpp
    ui/frames/inner/advanced_partition_frame.h
    ui/frames/inner/edit_partition_frame.cpp
    ui/frames/inner/edit_partition_frame.h
    ui/frames/inner/full_disk_frame.cpp
    ui/frames/inner/full_disk_frame.h
    ui/frames/inner/full_disk_encrypt_frame.h
    ui/frames/inner/full_disk_encrypt_frame.cpp
    ui/frames/inner/install_progress_slide_frame.cpp
    ui/frames/inner/install_progress_slide_frame.h
    ui/frames/inner/new_partition_frame.cpp
    ui/frames/inner/new_partition_frame.h
    ui/frames/inner/new_table_loading_frame.cpp
    ui/frames/inner/new_table_loading_frame.h
    ui/frames/inner/new_table_warning_frame.cpp
    ui/frames/inner/new_table_warning_frame.h
    ui/frames/inner/partition_loading_frame.cpp
    ui/frames/inner/partition_loading_frame.h
    ui/frames/inner/partition_number_limitation_frame.cpp
    ui/frames/inner/partition_number_limitation_frame.h
    ui/frames/inner/partition_table_warning_frame.cpp
    ui/frames/inner/partition_table_warning_frame.h
    ui/frames/inner/prepare_install_frame.cpp
    ui/frames/inner/prepare_install_frame.h
    ui/frames/inner/select_bootloader_frame.cpp
    ui/frames/inner/select_bootloader_frame.h
    ui/frames/inner/select_language_frame.cpp
    ui/frames/inner/select_language_frame.h
    ui/frames/inner/simple_partition_frame.cpp
    ui/frames/inner/simple_partition_frame.h
    ui/frames/inner/system_info_avatar_frame.cpp
    ui/frames/inner/system_info_avatar_frame.h
    ui/frames/inner/system_info_form_frame.cpp
    ui/frames/inner/system_info_form_frame.h
    ui/frames/inner/system_info_keyboard_frame.cpp
    ui/frames/inner/system_info_keyboard_frame.h
    ui/frames/inner/user_agreement_frame.h
    ui/frames/inner/user_agreement_frame.cpp
    ui/frames/timezone_frame.cpp
    ui/frames/timezone_frame.h

    ui/frames/confirm_quit_frame.cpp
    ui/frames/confirm_quit_frame.h
    ui/frames/control_panel_frame.cpp
    ui/frames/control_panel_frame.h
    ui/frames/disk_space_insufficient_frame.cpp
    ui/frames/disk_space_insufficient_frame.h
    ui/frames/first_boot_loading_frame.cpp
    ui/frames/first_boot_loading_frame.h
    ui/frames/install_failed_frame.cpp
    ui/frames/install_failed_frame.h
    ui/frames/install_progress_frame.cpp
    ui/frames/install_progress_frame.h
    ui/frames/install_success_frame.cpp
    ui/frames/install_success_frame.h
    ui/frames/partition_frame.cpp
    ui/frames/partition_frame.h
    ui/frames/privilege_error_frame.cpp
    ui/frames/privilege_error_frame.h
    ui/frames/language_frame.h
    ui/frames/language_frame.cpp
    ui/frames/system_info_frame.cpp
    ui/frames/system_info_frame.h
    ui/frames/virtual_machine_frame.cpp
    ui/frames/virtual_machine_frame.h
    ui/frames/networkframe.h
    ui/frames/networkframe.cpp
    ui/frames/control_platform_frame.h
    ui/frames/control_platform_frame.cpp
    ui/frames/dynamic_disk_warning_frame.h
    ui/frames/dynamic_disk_warning_frame.cpp

    ui/models/bootloader_list_model.cpp
    ui/models/bootloader_list_model.h
    ui/models/fs_model.cpp
    ui/models/fs_model.h
    ui/models/keyboard_layout_model.cpp
    ui/models/keyboard_layout_model.h
    ui/models/keyboard_layout_variant_model.cpp
    ui/models/keyboard_layout_variant_model.h
    ui/models/language_list_model.cpp
    ui/models/language_list_model.h
    ui/models/mount_point_model.cpp
    ui/models/mount_point_model.h
    ui/models/partition_model.cpp
    ui/models/partition_model.h
    ui/models/partition_type_model.cpp
    ui/models/partition_type_model.h
    ui/models/control_platform_region_model.h
    ui/models/control_platform_region_model.cpp

    ui/utils/widget_util.cpp
    ui/utils/widget_util.h
    ui/utils/xutil.cpp
    ui/utils/xutil.h
    ui/utils/keyboardmonitor.h
    ui/utils/keyboardmonitor.cpp

    ui/views/frameless_list_view.cpp
    ui/views/frameless_list_view.h
    ui/views/pointer_list_view.cpp
    ui/views/pointer_list_view.h

    ui/widgets/advanced_partition_button.cpp
    ui/widgets/advanced_partition_button.h
    ui/widgets/advanced_partition_error_label.cpp
    ui/widgets/advanced_partition_error_label.h
    ui/widgets/avatar_button.cpp
    ui/widgets/avatar_button.h
    ui/widgets/comment_label.cpp
    ui/widgets/comment_label.h
    ui/widgets/device_model_label.cpp
    ui/widgets/device_model_label.h
    ui/widgets/expanded_nav_button.cpp
    ui/widgets/expanded_nav_button.h
    ui/widgets/flat_button.cpp
    ui/widgets/flat_button.h
    ui/widgets/icon_button.cpp
    ui/widgets/icon_button.h
    ui/widgets/line_edit.cpp
    ui/widgets/line_edit.h
    ui/widgets/nav_button.cpp
    ui/widgets/nav_button.h
    ui/widgets/page_indicator.cpp
    ui/widgets/page_indicator.h
    ui/widgets/partition_size_slider.cpp
    ui/widgets/partition_size_slider.h
    ui/widgets/pointer_button.cpp
    ui/widgets/pointer_button.h
    ui/widgets/popup_menu.cpp
    ui/widgets/popup_menu.h
    ui/widgets/qr_widget.cpp
    ui/widgets/qr_widget.h
    ui/widgets/rounded_progress_bar.cpp
    ui/widgets/rounded_progress_bar.h
    ui/widgets/simple_disk_button.cpp
    ui/widgets/simple_disk_button.h
    ui/widgets/simple_partition_button.cpp
    ui/widgets/simple_partition_button.h
    ui/widgets/spinner_label.cpp
    ui/widgets/spinner_label.h
    ui/widgets/system_info_tip.cpp
    ui/widgets/system_info_tip.h
    ui/widgets/table_combo_box.cpp
    ui/widgets/table_combo_box.h
    ui/widgets/timezone_map.cpp
    ui/widgets/timezone_map.h
    ui/widgets/title_label.cpp
    ui/widgets/title_label.h
    ui/widgets/tooltip_pin.cpp
    ui/widgets/tooltip_pin.h
    ui/widgets/wallpaper_item.cpp
    ui/widgets/wallpaper_item.h

    ui/xrandr/multi_head_manager.cpp
    ui/xrandr/multi_head_manager.h
    ui/xrandr/multi_head_worker.cpp
    ui/xrandr/multi_head_worker.h
    ui/xrandr/xrandr_wrapper.cpp
    ui/xrandr/xrandr_wrapper.h

    ui/images/images.qrc
    ui/styles/styles.qrc
    license/license.qrc

    # Required by SpinnerLabel.
    ui/widgets/spinner/spinner.qrc
    )

set(ROOT_UNITTEST_FILES
    #    partman/operation_test.cpp
    #    partman/partition_usage_test.cpp
    #    partman/os_prober_test.cpp
    #    partman/partition_manager_test.cpp
    partman/libparted_util_test.cpp
    )

set(UNITTEST_FILES
    base/command_test.cpp
    base/file_util_test.cpp
    base/string_util_test.cpp

    partman/operation_test.cpp
    partman/partition_test.cpp

    sysinfo/dev_disk_test.cpp
    sysinfo/iso3166_test.cpp
    sysinfo/keyboard_test.cpp
    sysinfo/proc_meminfo_test.cpp
    sysinfo/proc_mounts_test.cpp
    sysinfo/proc_partitions_test.cpp
    sysinfo/proc_swaps_test.cpp
    sysinfo/timezone_test.cpp
    sysinfo/users_test.cpp
    sysinfo/validate_hostname_test.cpp
    sysinfo/validate_password_test.cpp
    sysinfo/validate_username_test.cpp

    ui/delegates/installer_args_parser_test.cpp
    ui/delegates/install_slide_frame_util_test.cpp
    ui/delegates/timezone_map_util_test.cpp
    )

set(QtCore_LIBS Qt5::Core)
set(Qt_LIBS
    Qt5::Gui
    Qt5::Network
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::Xml
    Qt5::Svg
    Qt5::DBus
    )

set(LINK_LIBS
    ${Qt_LIBS}
    ${Parted_LIBRARIES}
    ${X11_LIBRARIES}
    ${X11EXT_LIBRARIES}
    ${X11TST_LIBRARIES}
    ${X11RandR_LIBRARIES}

    global_shortcut
    qrencode
    qtermwidget5
    zlib_wrapper
    Xi
    )


# Main executable
# Generate .qm files from .ts files.
file(GLOB INSTALLER_TRANSLATION_TS
     ${CMAKE_SOURCE_DIR}/i18n/installer-*.ts)
set_source_files_properties(${INSTALLER_TRANSLATION_TS}
                            PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/i18n)
qt5_add_translation(INSTALLER_TRANSLATION_QM ${INSTALLER_TRANSLATION_TS})
add_custom_target(installer-qm-update DEPENDS ${INSTALLER_TRANSLATION_QM})

add_executable(deepin-installer
               app/deepin_installer.cpp
               ui/main_window.cpp
               ui/main_window.h

               ${BASE_FILES}
               ${PARTMAN_FILES}
               ${SERVICE_FILES}
               ${SYSINFO_FILES}
               ${UI_FILES}

               # Also include color-schems used in qtermwidget.
               third_party/qtermwidget/lib/color-schemes/color-schemes.qrc
               third_party/qtermwidget/lib/kb-layouts/kb-layouts.qrc
               )

target_link_libraries(deepin-installer ${LINK_LIBS})
add_dependencies(deepin-installer installer-qm-update)


# Set/get ini files.
add_executable(deepin-installer-settings
               app/deepin_installer_settings.cpp)

target_link_libraries(deepin-installer-settings ${QtCore_LIBS})

add_executable(deepin-installer-simpleini
               app/deepin_installer_simpleini.cpp)

target_link_libraries(deepin-installer-simpleini
                      ${QtCore_LIBS}
                      simpleini)


# Reboot setup
add_executable(deepin-installer-first-boot
               app/deepin_installer_first_boot.cpp
               ui/first_boot_setup_window.cpp
               ui/first_boot_setup_window.h

               ${BASE_FILES}
               ${PARTMAN_FILES}
               ${SERVICE_FILES}
               ${SYSINFO_FILES}
               ${UI_FILES}
               )
target_link_libraries(deepin-installer-first-boot
                      ${LINK_LIBS}
                      global_shortcut
                      )

add_executable(deepin-installer-user-form
               app/deepin_installer_user_form.cpp
               ${BASE_FILES}

               service/settings_manager.cpp
               service/settings_manager.h
               service/settings_name.h

               sysinfo/keyboard.cpp
               sysinfo/keyboard.h
               sysinfo/users.cpp
               sysinfo/users.h
               sysinfo/validate_hostname.cpp
               sysinfo/validate_hostname.h
               sysinfo/validate_password.cpp
               sysinfo/validate_password.h
               sysinfo/validate_username.cpp
               sysinfo/validate_username.h

               ui/user_form_window.cpp
               ui/user_form_window.h
               ui/delegates/avatar_list_delegate.cpp
               ui/delegates/avatar_list_delegate.h
               ui/delegates/frameless_list_delegate.cpp
               ui/delegates/frameless_list_delegate.h
               ui/frames/system_info_frame.cpp
               ui/frames/system_info_frame.h
               ui/frames/inner/system_info_avatar_frame.cpp
               ui/frames/inner/system_info_avatar_frame.h
               ui/frames/inner/system_info_form_frame.cpp
               ui/frames/inner/system_info_form_frame.h
               ui/frames/inner/system_info_keyboard_frame.cpp
               ui/frames/inner/system_info_keyboard_frame.h
               ui/models/keyboard_layout_model.cpp
               ui/models/keyboard_layout_model.h
               ui/models/keyboard_layout_variant_model.cpp
               ui/models/keyboard_layout_variant_model.h
               ui/views/frameless_list_view.cpp
               ui/views/frameless_list_view.h
               ui/views/pointer_list_view.cpp
               ui/views/pointer_list_view.h
               ui/widgets/avatar_button.cpp
               ui/widgets/avatar_button.h
               ui/widgets/comment_label.cpp
               ui/widgets/comment_label.h
               ui/widgets/flat_button.cpp
               ui/widgets/flat_button.h
               ui/widgets/line_edit.cpp
               ui/widgets/line_edit.h
               ui/widgets/nav_button.cpp
               ui/widgets/nav_button.h
               ui/widgets/pointer_button.cpp
               ui/widgets/pointer_button.h
               ui/widgets/system_info_tip.cpp
               ui/widgets/system_info_tip.h
               ui/widgets/title_label.cpp
               ui/widgets/title_label.h
               ui/utils/widget_util.cpp
               ui/utils/widget_util.h
               ui/utils/xutil.cpp
               ui/utils/xutil.h
               ui/utils/keyboardmonitor.h
               ui/utils/keyboardmonitor.cpp

               ui/images/images.qrc
               ui/styles/styles.qrc
               license/license.qrc
               )
target_link_libraries(deepin-installer-user-form ${LINK_LIBS})

# deepin-installer-oem
add_executable(deepin-installer-oem
               app/deepin_installer_oem.cpp
               oem/oem_window.cpp
               oem/oem_window.h

               service/settings_name.h
               service/system_language.cpp
               service/system_language.h

               ${BASE_FILES}
               ${OEM_FILES}
               ${SYSINFO_FILES}
               )
target_link_libraries(deepin-installer-oem ${Qt_LIBS})

add_executable(deepin-installer-unsquashfs

               app/deepin_installer_unsquashfs.cpp
               ${BASE_FILES}
               )
target_link_libraries(deepin-installer-unsquashfs ${Qt_LIBS})

# xrandr-switchy
add_executable(deepin-installer-xrandr-switchy
               ui/tests/xrandr_switchy.cpp

               ${BASE_FILES}
               ui/xrandr/xrandr_wrapper.cpp
               ui/xrandr/xrandr_wrapper.h
               )
target_link_libraries(deepin-installer-xrandr-switchy
                      ${QtCore_LIBS}

                      ${X11_LIBRARIES}
                      ${X11EXT_LIBRARIES}
                      ${X11TST_LIBRARIES}
                      ${X11RandR_LIBRARIES}
                      )

# Unsquashfs progress
#add_executable(deepin-installer-unsquashfs-progress
#
#               app/unsquashfs_progress.cpp
#               ${BASE_FILES}
#               )
#target_link_libraries(deepin-installer-unsquashfs-progress ${QtCore_LIBS})


# test reboot system
add_executable(reboot-system
               app/reboot_system.cpp

               ${BASE_FILES}
               service/power_manager.cpp
               service/power_manager.h
               )
target_link_libraries(reboot-system ${QtCore_LIBS})

# Combobox test
add_executable(combobox-test
               ui/tests/combobox_test.cpp

               ${BASE_FILES}
               ui/delegates/table_combo_box_delegate.cpp
               ui/delegates/table_combo_box_delegate.h
               ui/utils/widget_util.cpp
               ui/utils/widget_util.h
               ui/widgets/table_combo_box.cpp
               ui/widgets/table_combo_box.h

               ui/images/images.qrc
               ui/styles/styles.qrc
               license/license.qrc
               )
target_link_libraries(combobox-test ${Qt_LIBS})


# locale manager test
add_executable(locale-manager-test
               service/timezone_manager_test.cpp

               ${BASE_FILES}

               service/backend/wifi_inspect_worker.cpp
               service/backend/wifi_inspect_worker.h
               service/backend/geoip_request_worker.cpp
               service/backend/geoip_request_worker.h
               service/timezone_manager.cpp
               service/timezone_manager.h

               ${SYSINFO_FILES}
               )
target_link_libraries(locale-manager-test ${Qt_LIBS})


# QR widget test
add_executable(qr-widget-test
               ui/tests/qr_widget_test.cpp

               base/file_util.cpp
               base/file_util.h
               third_party/zlib_wrapper/zip_util.cpp
               third_party/zlib_wrapper/zip_util.h
               ui/widgets/qr_widget.cpp
               ui/widgets/qr_widget.h
               )
target_link_libraries(qr-widget-test
                      ${Qt_LIBS}
                      ${ZLib_LIBRARIES}

                      qrencode
                      )

# progress bar test
add_executable(qprogressbar_test
               ui/tests/progressbar_test.cpp
               ui/widgets/rounded_progress_bar.cpp
               ui/widgets/rounded_progress_bar.h
               )
target_link_libraries(qprogressbar_test ${Qt_LIBS})

# spinner test
add_executable(spinner-test
               ui/tests/spinner_test.cpp

               ui/widgets/spinner_label.cpp
               ui/widgets/spinner_label.h

               ui/widgets/spinner/spinner.qrc
               )
target_link_libraries(spinner-test ${Qt_LIBS})


# system info tip test
add_executable(system_info_tip_test
               ui/tests/system_info_tip_test.cpp

               ui/widgets/system_info_tip.cpp
               ui/widgets/system_info_tip.h

               ${BASE_FILES}
               ui/images/images.qrc
               ui/styles/styles.qrc
               license/license.qrc
               )
target_link_libraries(system_info_tip_test ${Qt_LIBS})

# timezone frame test
add_executable(timezone-frame-test
               ui/tests/timezone_frame_test/timezone_frame_test.cpp

               ${BASE_FILES}

               service/settings_manager.cpp
               service/settings_manager.h
               service/settings_name.h
               service/timezone_manager.cpp
               service/timezone_manager.h
               service/backend/geoip_request_worker.cpp
               service/backend/geoip_request_worker.h
               service/backend/wifi_inspect_worker.cpp
               service/backend/wifi_inspect_worker.h

               partman/os_prober.cpp
               partman/os_prober.h
               partman/structs.cpp
               partman/structs.h

               sysinfo/machine.cpp
               sysinfo/machine.h
               sysinfo/timezone.cpp
               sysinfo/timezone.h
               sysinfo/users.cpp
               sysinfo/users.h

               ui/utils/widget_util.h
               ui/utils/widget_util.cpp

               ui/delegates/popup_menu_delegate.cpp
               ui/delegates/popup_menu_delegate.h
               ui/delegates/timezone_map_util.cpp
               ui/delegates/timezone_map_util.h
               ui/frames/consts.h
               ui/frames/timezone_frame.cpp
               ui/frames/timezone_frame.h
               ui/widgets/comment_label.cpp
               ui/widgets/comment_label.h
               ui/widgets/flat_button.cpp
               ui/widgets/flat_button.h
               ui/widgets/nav_button.cpp
               ui/widgets/nav_button.h
               ui/widgets/pointer_button.cpp
               ui/widgets/pointer_button.h
               ui/widgets/popup_menu.cpp
               ui/widgets/popup_menu.h
               ui/widgets/timezone_map.cpp
               ui/widgets/timezone_map.h
               ui/widgets/title_label.cpp
               ui/widgets/title_label.h
               ui/widgets/tooltip_pin.cpp
               ui/widgets/tooltip_pin.h

               ui/images/images.qrc
               ui/styles/styles.qrc
               license/license.qrc
               )
target_link_libraries(timezone-frame-test ${Qt_LIBS})

add_executable(ctl-btn-test
               ui/tests/ctl_btn_test.cpp

               base/file_util.cpp
               base/file_util.h
               ui/widgets/pointer_button.cpp
               ui/widgets/pointer_button.h

               ui/images/images.qrc
               ui/styles/styles.qrc
               license/license.qrc
               )
target_link_libraries(ctl-btn-test ${Qt_LIBS})

add_executable(partition-util-test
               ui/tests/partition_util_test.cpp
               )
target_link_libraries(partition-util-test ${Qt_LIBS})

# Unittest
add_executable(deepin-installer-tests
               app/unittest_main.cpp

               ${BASE_FILES}
               ${PARTMAN_FILES}
               ${SYSINFO_FILES}
               ${UNITTEST_FILES}

               service/settings_manager.cpp
               service/settings_manager.h

               ui/delegates/installer_args_parser.cpp
               ui/delegates/installer_args_parser.h
               ui/delegates/install_slide_frame_util.cpp
               ui/delegates/install_slide_frame_util.h
               ui/delegates/timezone_map_util.cpp
               ui/delegates/timezone_map_util.h
               )
target_link_libraries(deepin-installer-tests
                      ${LINK_LIBS}
                      gtest
                      )

# Unittest with root privilege
add_executable(deepin-installer-root-tests
               app/root_unittest_main.cpp

               ${BASE_FILES}
               ${PARTMAN_FILES}
               ${SYSINFO_FILES}
               ${ROOT_UNITTEST_FILES}
               )
target_link_libraries(deepin-installer-root-tests
                      ${LINK_LIBS}
                      gtest
                      )


# Unsuqashfs progress window test
add_definitions("-DUNSQUASHFS_SH=\"${CMAKE_CURRENT_SOURCE_DIR}/misc/unsquashfs_gui/unsquashfs.sh\"")
add_executable(unsquashfs_gui

               ui/tests/unsquashfs_gui/unsquashfs_gui.cpp
               ui/tests/unsquashfs_gui/unsquashfs_progress_window.cpp
               ui/tests/unsquashfs_gui/unsquashfs_progress_window.h

               ${BASE_FILES}
               ui/tests/unsquashfs_gui/worker.cpp
               ui/tests/unsquashfs_gui/worker.h
               )
target_link_libraries(unsquashfs_gui ${Qt_LIBS})


## Installation
install(PROGRAMS
        ${CMAKE_CURRENT_SOURCE_DIR}/app/deepin-installer-auto-install
        ${CMAKE_CURRENT_SOURCE_DIR}/app/deepin-installer-first-boot-pkexec
        ${CMAKE_CURRENT_SOURCE_DIR}/app/deepin-installer-pkexec
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        )

install(TARGETS
        deepin-installer
        deepin-installer-first-boot
        deepin-installer-oem
        deepin-installer-settings
        deepin-installer-simpleini
        deepin-installer-unsquashfs
        deepin-installer-user-form
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        )

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/i18n
    DESTINATION ${INSTALLER_SHARE_DIR}
    FILES_MATCHING PATTERN "*"
)
