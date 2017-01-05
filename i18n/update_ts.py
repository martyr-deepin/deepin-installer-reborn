#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Update i18n/installer.ts

import os
import sys

# Relative path to installer ts file.
INSTALLER_TS="i18n/installer.ts"

# Relative path to oem ts file.
OEM_TS="i18n/oem.ts"


def usage():
    """Print usage message"""
    print("Usage: %s installer|oem\n" % sys.argv[0])
    print("installer - generate installer.ts")
    print("oem - generate oem.ts")

def generate_ts(ts_path):
    """Generate new ts file.

    Scan source files in `ui` module recursively.
    And save result to |ts_path|.
    """
    if ts_path == INSTALLER_TS:
        paths = (
            "delegates",
            "first_boot_setup_window.cpp"
            "first_boot_setup_window.h",
            "frames",
            "main_window.cpp"
            "main_window.h",
            "models",
            "views",
            "widgets",
        )
        cmd = " ".join((
            "lupdate -recursive",
            " ".join(paths),
            "-ts", ts_path,
            ))
        return os.system(cmd) == 0
    elif ts_path == OEM_TS:
        # Only include ui/oem folder.
        paths = "ui/oem"
        cmd = " ".join((
            "lupdate -recursive",
            paths,
            "-ts", ts_path,
            ))
        return os.system(cmd) == 0
    else:
        # We shall never reach here.
        return False

def fix_lupdate_namespace(ts_path):
    """Fix context error in ts file.
    
    lupdate may be unable to distinguish namespace declaration in source file.
    """

    tmp_ts_path = ts_path + ".tmp"
    with open(ts_path) as fin, open(tmp_ts_path, "w") as fout:
        for line in fin:
            if "<name>" in line and \
                    "QObject" not in line and \
                    "installer:" not in line:
                # Append namespace.
                line = line.replace("<name>", "<name>installer::")
            fout.write(line)

    # Override default ts file.
    os.rename(tmp_ts_path, ts_path)

def main():
    # Make sure that PWD is root of source repo.
    if not os.path.isdir("i18n"):
        print("Run script in parent folder of `i18n`")
        sys.exit(1)
    if len(sys.argv) != 2:
        usage()
        sys.exit(1)

    if sys.argv[1] == "installer":
        if not generate_ts(INSTALLER_TS):
            print("Failed to generate %s file!" % INSTALLER_TS)
            sys.exit(1)

        fix_lupdate_namespace(INSTALLER_TS)
    elif sys.argv[1] == "oem":
        if not generate_ts(OEM_TS):
            print("Failed to generate %s file!" % INSTALLER_TS)
            sys.exit(1)

        fix_lupdate_namespace(OEM_TS)
    else:
        usage()
        sys.exit(1)


if __name__ == "__main__":
    main()
