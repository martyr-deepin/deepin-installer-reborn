if (${INSTALLER_BUILD_ARCH} MATCHES "x86")
  install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn/resources/slide
      PATTERN "zh_CN/20-wps.png" EXCLUDE
      PATTERN "zh_CN/21-youdao-dict.png" EXCLUDE
      PATTERN "CMakeLists.txt" EXCLUDE
  )
elseif(${INSTALLER_BUILD_ARCH} MATCHES "sw")
  install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn/resources/slide
      PATTERN "zh_CN/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_CN/17-tencent-qq.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_TW/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/17-tencent-qq.png" EXCLUDE
      PATTERN "zh_TW/18-netease-cloud-music.png" EXCLUDE
      PATTERN "CMakeLists.txt" EXCLUDE
  )
elseif(${INSTALLER_BUILD_ARCH} MATCHES "mips")
  install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION  ${CMAKE_INSTALL_PREFIX}/share/deepin-installer-reborn/resources/slide
      PATTERN "zh_CN/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_CN/17-tencent-qq.png" EXCLUDE
      PATTERN "zh_CN/18-netease-cloud-music.png" EXCLUDE
      PATTERN "zh_TW/16-wps-youdao-dict.png" EXCLUDE
      PATTERN "zh_TW/17-tencent-qq.png" EXCLUDE
      PATTERN "zh_TW/18-netease-cloud-music.png" EXCLUDE
      PATTERN "CMakeLists.txt" EXCLUDE
  )
else()
  message(FATAL_ERROR "Not supportted architecture: ${INSTALLER_BUILD_ARCH}")
endif ()
