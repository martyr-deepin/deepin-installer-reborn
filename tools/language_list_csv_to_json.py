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

# Convert support-language-list.ini to languages.json

import configparser
import json
import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: %s ini-file" % sys.argv[0])
        sys.exit(1)

    ini_file = sys.argv[1]
    csv = configparser.ConfigParser()
    csv.read(ini_file)
    lang_list = []
    for section_name in csv.sections():
        section = { "locale": section_name }
        section["name"] = csv.get(section_name, "name")
        section["local_name"] = csv.get(section_name, "local")
        lang_list.append(section)
    output = json.dumps(lang_list, ensure_ascii=False, indent=2)
    print(output)


if __name__ == "__main__":
    main()

