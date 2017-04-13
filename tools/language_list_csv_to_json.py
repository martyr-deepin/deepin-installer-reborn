#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

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

