# Install resources/ folder, excluding desktop file and icons.
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION ${INSTALLER_SHARE_DIR}
    FILES_MATCHING PATTERN "*"
    PATTERN "CMakeLists.txt" EXCLUDE
    PATTERN "com.*.policy" EXCLUDE
    PATTERN "deepin-installer-oem*" EXCLUDE
    PATTERN "deepin-installer-reborn*" EXCLUDE
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