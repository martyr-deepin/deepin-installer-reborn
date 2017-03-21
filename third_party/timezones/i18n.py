#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

import os
import shutil
import subprocess

HEADER_FILE = "system-config-date-timezones.h"
POT_FILE="system-config-date-timezones.pot"
PO_FILE="deepin-installer-timezones.po"

def generate_header_file():
    """Generate C header file from zone.tab"""
    zone_tab = "/usr/share/zoneinfo/zone.tab"
    zone_list = []
    with open(zone_tab) as zone_fh:
        for line in zone_fh:
            if not line:
                continue
            if line.startswith("#"):
                continue
            items = line.split("\t")
            if len(items) < 3:
                print("Skip line:", line)
                continue
            zone_list.append(items[2].strip())
    with open(HEADER_FILE, "w") as header_fh:
        for zone in sorted(zone_list):
            header_fh.write('N_("%s");\n' % zone)

def generate_pot_file():
    """Generate pot file from header file"""
    cmd = [
            "xgettext",
            "--from-code=UTF-8",
            "--keyword=N_",
            "--output", POT_FILE,
            HEADER_FILE,
            ]
    subprocess.call(cmd)

def update_pot_charset():
    """Update CHARSET in pot file to UTF-8"""
    content = open(POT_FILE).read()
    content = content.replace("CHARSET", "UTF-8")
    with open(POT_FILE, "w") as fh:
        fh.write(content)

if __name__ == "__main__":
    generate_header_file()
    generate_pot_file()
    update_pot_charset()
