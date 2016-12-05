#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Rename filename by replacing dashes with underscores

import os

def main():
    files = os.listdir()
    for file in files:
        new_file = file.replace('-', '_')
        if file != new_file:
            print("Rename:", file, "->", new_file)
            os.rename(file, new_file)

if __name__ == "__main__":
    main()
