#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

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
