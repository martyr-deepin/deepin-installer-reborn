install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/deepin-installer.conf
    DESTINATION /etc/live/config.conf.d/
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/live-boot
    DESTINATION /lib/live/boot
    USE_SOURCE_PERMISSIONS
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/live-config
    DESTINATION /lib/live/config
    USE_SOURCE_PERMISSIONS
)