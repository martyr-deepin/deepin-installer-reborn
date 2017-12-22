install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/deepin-installer.conf
    DESTINATION /etc/live/config.conf.d/
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/live-boot/
    DESTINATION /lib/live/boot
    FILES_MATCHING PATTERN "*"
    PERMISSIONS OWNER_READ OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lightdm-installer-greeter.desktop
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/xgreeters/
)

# Startup with `lightdm` on x86 architectures.
# Startup with `systemd` on non-x86 architectures.
if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "^x86*")
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/live-config/
        DESTINATION /lib/live/config
        FILES_MATCHING PATTERN "*"
        PERMISSIONS OWNER_READ OWNER_EXECUTE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE
    )
else()
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/systemd/
        DESTINATION /lib/systemd/system
        FILES_MATCHING PATTERN "*"
    )
endif()
