#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Update i18n/installer.ts

import os
import sys

def generate_ts():
    """Generate new ts file.

    Scan source files in `ui` module recursively.
    And save result to i18n/installer.ts.
    """
    return os.system("lupdate -recursive ui -ts i18n/installer.ts") == 0

def fix_lupdate_namespace():
    """Fix context error in ts file.
    
    lupdate may be unable to distinguish namespace declaration in source file.
    """

    pathin = "i18n/installer.ts"
    pathout = "i18n/installer.ts.tmp"
    with open(pathin) as fin, open(pathout, "w") as fout:
        for line in fin:
            if "<name>" in line and \
                    "QObject" not in line and \
                    "installer:" not in line:
                # Append namespace.
                line = line.replace("<name>", "<name>installer::")
            fout.write(line)

    # Override default ts file.
    os.rename(pathout, pathin)

def main():
    # Check PWD is root of source repo.
    if not os.path.isdir("i18n"):
        print("Run script in parent folder of `i18n`")
        sys.exit(1)
    if not generate_ts():
        print("Failed to generate ts file!")
        sys.exit(1)

    fix_lupdate_namespace()


if __name__ == "__main__":
    main()
