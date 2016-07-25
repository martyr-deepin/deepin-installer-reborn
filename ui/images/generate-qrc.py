#!/usr/bin/env python3

import os
import sys

def generate(prefix, folders, output):
    with open(output, "w") as fd:
        fd.write("<RCC>\n")
        fd.write('  <qresource prefix="%s">\n' % prefix)
        for folder in folders:
            for dir, dirs, files in os.walk(folder):
                for file in files:
                    #item = os.path.join(dir, file)
                    fd.write("    <file>%s</file>\n" % file)
        fd.write("  </qresource>\n")
        fd.write("</RCC>\n")

def main():
    prefix="/images"
    output = "images.qrc"
    generate(prefix, ['.'], output)

if __name__ == "__main__":
    main()
