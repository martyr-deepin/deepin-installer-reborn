#!/usr/bin/env python3

import os
import shutil

def main():
    tpl = """
gettext_process_po_files(%s ALL INSTALL_DESTINATION share/locale
    PO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/deepin-installer-timezones.po)"""
    po_files = os.listdir(".")
    for po in po_files:
        if po.endswith(".po"):
            name = os.path.splitext(po)[0]
            os.mkdir(name)
            dest = name + "/deepin-installer-timezones.po"
            shutil.move(po, dest)
            cmake_file = os.path.join(name, "CMakeLists.txt")
            with open(cmake_file, "w") as fh:
                fh.write(tpl % name)
            with open("CMakeLists.txt", "a") as fh:
                fh.write(name)
                fh.write("\n")



if __name__ == "__main__":
    main()
