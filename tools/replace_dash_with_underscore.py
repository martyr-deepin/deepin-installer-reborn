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
