find_package(PkgConfig REQUIRED)
find_package(DtkCMake REQUIRED)

if (${INSTALLER_BUILD_ARCH} MATCHES "x86")
  if (OS_DEEPIN_PROFESSIONAL)
    install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer/resources/slide
      PATTERN "zh_CN/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "zh_CN/21-youdao-dict.png" EXCLUDE
      PATTERN "zh_CN/03-deepin-movie.png" EXCLUDE
      PATTERN "zh_CN/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "zh_CN/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_CN/20-wps.png" EXCLUDE
      PATTERN "zh_CN/25-desktop-automerge.png" EXCLUDE
      PATTERN "zh_TW/03-deepin-movie.png" EXCLUDE
      PATTERN "zh_TW/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "zh_TW/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_TW/20-wps.png" EXCLUDE
      PATTERN "zh_TW/25-desktop-automerge.png" EXCLUDE
      PATTERN "default/25-desktop-automerge.png" EXCLUDE
      PATTERN "default/03-deepin-movie.png" EXCLUDE
      PATTERN "default/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "default/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "default/18-netease-cloud-music.png" EXCLUDE
      PATTERN "default/20-wps.png" EXCLUDE
      PATTERN "default/17-skype.png" EXCLUDE
      PATTERN "default/18-spotify.png" EXCLUDE
      PATTERN "CMakeLists.txt" EXCLUDE
    )
  else()
    install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer/resources/slide
      PATTERN "zh_CN/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "zh_CN/20-wps.png" EXCLUDE
      PATTERN "zh_CN/21-youdao-dict.png" EXCLUDE
      PATTERN "zh_CN/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "CMakeLists.txt" EXCLUDE
    )
  endif()
elseif(${INSTALLER_BUILD_ARCH} MATCHES "sw")
  install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer/resources/slide
      PATTERN "default/02-deepin-app-store.png" EXCLUDE
      PATTERN "default/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "default/12-deepin-dock.png" EXCLUDE
      PATTERN "default/17-skype.png" EXCLUDE
      PATTERN "default/18-spotify.png" EXCLUDE
      PATTERN "zh_CN/02-deepin-app-store.png" EXCLUDE
      PATTERN "zh_CN/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "zh_CN/12-deepin-dock.png" EXCLUDE
      PATTERN "zh_CN/17-tencent-qq.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_CN/21-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/02-deepin-app-store.png" EXCLUDE
      PATTERN "zh_TW/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "zh_CN/03-deepin-movie.png" EXCLUDE
      PATTERN "zh_CN/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "zh_CN/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_CN/20-wps.png" EXCLUDE
      PATTERN "zh_CN/25-desktop-automerge.png" EXCLUDE
      PATTERN "zh_TW/03-deepin-movie.png" EXCLUDE
      PATTERN "zh_TW/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "zh_TW/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_TW/20-wps.png" EXCLUDE
      PATTERN "zh_TW/25-desktop-automerge.png" EXCLUDE
      PATTERN "default/03-deepin-movie.png" EXCLUDE
      PATTERN "default/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "default/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "default/18-netease-cloud-music.png" EXCLUDE
      PATTERN "default/20-wps.png" EXCLUDE
      PATTERN "default/25-desktop-automerge.png" EXCLUDE
      PATTERN "CMakeLists.txt" EXCLUDE
  )
elseif(${INSTALLER_BUILD_ARCH} MATCHES "mips")
  install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer/resources/slide
      PATTERN "default/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "default/17-skype.png" EXCLUDE
      PATTERN "default/18-Spotify.png" EXCLUDE
      PATTERN "zh_CN/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "zh_CN/17-tencent-qq.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_CN/21-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "zh_CN/03-deepin-movie.png" EXCLUDE
      PATTERN "zh_CN/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "zh_CN/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_CN/20-wps.png" EXCLUDE
      PATTERN "zh_CN/25-desktop-automerge.png" EXCLUDE
      PATTERN "zh_TW/03-deepin-movie.png" EXCLUDE
      PATTERN "zh_TW/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "zh_TW/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_TW/20-wps.png" EXCLUDE
      PATTERN "zh_TW/25-desktop-automerge.png" EXCLUDE
      PATTERN "default/03-deepin-movie.png" EXCLUDE
      PATTERN "default/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "default/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "default/18-netease-cloud-music.png" EXCLUDE
      PATTERN "default/20-wps.png" EXCLUDE
      PATTERN "default/25-desktop-automerge.png" EXCLUDE
      PATTERN "CMakeLists.txt" EXCLUDE
  )
elseif(${INSTALLER_BUILD_ARCH} MATCHES "aarch64")
  install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer/resources/slide
      PATTERN "default/02-deepin-app-store.png" EXCLUDE
      PATTERN "default/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "default/17-skype.png" EXCLUDE
      PATTERN "default/18-spotify.png" EXCLUDE
      PATTERN "zh_CN/02-deepin-app-store.png" EXCLUDE
      PATTERN "zh_CN/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "zh_CN/17-tencent-qq.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_CN/21-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/02-deepin-app-store.png" EXCLUDE
      PATTERN "zh_TW/10-deepin-remote-assistance.png" EXCLUDE
      PATTERN "zh_CN/03-deepin-movie.png" EXCLUDE
      PATTERN "zh_CN/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "zh_CN/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_CN/20-wps.png" EXCLUDE
      PATTERN "zh_CN/25-desktop-automerge.png" EXCLUDE
      PATTERN "zh_TW/03-deepin-movie.png" EXCLUDE
      PATTERN "zh_TW/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "zh_TW/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_TW/20-wps.png" EXCLUDE
      PATTERN "zh_TW/25-desktop-automerge.png" EXCLUDE
      PATTERN "default/03-deepin-movie.png" EXCLUDE
      PATTERN "default/08-deepin-cloud-print.png" EXCLUDE
      PATTERN "default/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "default/18-netease-cloud-music.png" EXCLUDE
      PATTERN "default/20-wps.png" EXCLUDE
      PATTERN "default/25-desktop-automerge.png" EXCLUDE
      PATTERN "CMakeLists.txt" EXCLUDE
  )
else()
  message(FATAL_ERROR "Not supportted architecture: ${INSTALLER_BUILD_ARCH}")
endif ()
