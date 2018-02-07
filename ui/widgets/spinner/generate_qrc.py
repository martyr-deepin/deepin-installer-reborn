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

def generate(prefix, folders, output):
    with open(output, "w") as fd:
        fd.write("<RCC>\n")
        fd.write('  <qresource prefix="%s">\n' % prefix)
        for folder in folders:
            for dir, dirs, files in os.walk(folder):
                for file in files:
                    if file != output:
                        fd.write("    <file>%s</file>\n" % file)
        fd.write("  </qresource>\n")
        fd.write("</RCC>\n")

def main():
    prefix="/spinner"
    output = "spinner.qrc"
    generate(prefix, ['.'], output)

if __name__ == "__main__":
    main()
