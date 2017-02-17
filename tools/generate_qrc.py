#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Generate qrc file automatically

import os
import sys

def generate_qrc(prefix, input_folder, qrc_file):
    """Generate qrc file

    * prefix, default shallbe "/".
    * input_folder, resource folder to be scanned.
    * qrc_file, RCC content to be written to.
    """
    lines = []
    lines.append("<!DOCTYPE RCC>")
    lines.append('<RCC version="1.0">')
    if prefix:
        lines.append('  <qresource prefix="%s">' % prefix)
    else:
        lines.append("  <qresource>")
    input_folder_name_len = len(os.path.abspath(input_folder))
    for dir, dirs, files in os.walk(input_folder):
        for file in sorted(files):
            absfile = os.path.abspath(os.path.join(dir, file))
            # Ignore target qrc file
            if absfile == qrc_file:
                continue
            relative_file = absfile[input_folder_name_len+1:]
            lines.append("    <file>%s</file>" % relative_file)
    lines.append("  </qresource>")
    lines.append("</RCC>")
    with open(qrc_file, "w") as fh:
        fh.write("\n".join(lines))

def usage():
    print("Usage: %s prefix input-folder qrc-file" % sys.argv[0])

def main():
    if len(sys.argv) != 4:
        usage()
        sys.exit(1)
    prefix = sys.argv[1]
    input_folder = sys.argv[2]
    qrc_file = os.path.abspath(sys.argv[3])
    generate_qrc(prefix, input_folder, qrc_file)

if __name__ == "__main__":
    main()
