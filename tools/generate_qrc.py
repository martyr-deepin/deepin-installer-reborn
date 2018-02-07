#!/usr/bin/env python3
#
# Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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
