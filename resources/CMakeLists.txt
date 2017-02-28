# Install resources/ folder, excluding desktop file and icons.

add_subdirectory(slide)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/default_wallpaper.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/languages.json
    ${CMAKE_CURRENT_SOURCE_DIR}/timezone_alias
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn/resources
)

# Install PolicyKit file.
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/com.deepin.pkexec.installer.policy
    ${CMAKE_CURRENT_SOURCE_DIR}/com.deepin.pkexec.installer-first-boot.policy
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/polkit-1/actions/
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/deepin-installer-oem.desktop
    ${CMAKE_CURRENT_SOURCE_DIR}/deepin-installer-reborn.desktop
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/deepin-installer-oem.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/deepin-installer-reborn.svg
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps
)


# Install default_settings.ini based on current architecture.
set(INSTALLER_BUILD_ARCH "")
execute_process(COMMAND uname -m
                OUTPUT_VARIABLE INSTALLER_BUILD_ARCH)
message(${INSTALLER_BUILD_ARCH})

if (${INSTALLER_BUILD_ARCH} MATCHES "x86")
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/default_settings.ini
          DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn/resources
          )
elseif(${INSTALLER_BUILD_ARCH} MATCHES "sw")
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/sw_default_settings.ini
          DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn/resources
          RENAME default_settings.ini
          )
elseif(${INSTALLER_BUILD_ARCH} MATCHES "mips")
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/loongson_default_settings.ini
          DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn/resources
          RENAME default_settings.ini
          )
else()
  message(FATAL_ERROR "Not supportted architecture: ${INSTALLER_BUILD_ARCH}")
endif ()