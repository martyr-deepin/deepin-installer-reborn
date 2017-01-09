#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Update i18n/installer.ts and i18n/oem.ts

import json
import os
import sys

def get_language_list():
    """Parse language list and returns locale list."""
    path = "resources/languages.json"
    with open(path) as fh:
        obj = json.load(fh)
    if obj:
        return [lang["locale"] for lang in obj]
    else:
        return []

def generate_ts(ts_path, is_installer):
    """Generate new ts file.

    Scan source files in `ui` module recursively.
    And save result to |ts_path|.

    If |is_installer| is True, generate files for installer project.
    Else generate ts files for oem project.
    """
    if is_installer:
        paths = (
            "ui/delegates",
            "ui/first_boot_setup_window.cpp",
            "ui/first_boot_setup_window.h",
            "ui/frames",
            "ui/main_window.cpp",
            "ui/main_window.h",
            "ui/models",
            "ui/views",
            "ui/widgets",
        )
    else:
        # Only include ui/oem folder.
        paths = ["ui/oem"]

    # Add -I. option to solve namespace error
    cmd = " ".join((
        "lupdate -recursive -I.",
        " ".join(paths),
        "-ts", ts_path,
        ))
    os.system(cmd)

def main():
    # Make sure that PWD is root of source repo.
    if not os.path.isdir("i18n"):
        print("Run script in parent folder of `i18n`")
        sys.exit(1)
    
    lang_list = get_language_list()
    if lang_list:
        for lang in lang_list:
            ts_path = "i18n/installer-%s.ts" % lang
            generate_ts(ts_path, True)
        default_installer_ts = "i18n/installer.ts"
        generate_ts(default_installer_ts, True)
    else:
        print("Failed to parse language list")
        sys.exit(1)

    # Then, generate ts files for oem project.
    oem_ts = "i18n/oem-zh_CN.ts"
    generate_ts(oem_ts, False)

if __name__ == "__main__":
    main()
