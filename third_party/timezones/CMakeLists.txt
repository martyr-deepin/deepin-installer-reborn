
find_package(Gettext REQUIRED)

gettext_process_po_files(zh_CN PO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/zh_CN.po)
gettext_process_po_files(zh_TW PO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/zh_TW.po)
