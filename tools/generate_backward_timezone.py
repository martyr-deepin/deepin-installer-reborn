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

# Scan zone info in system and generate timezone alias map.

import json
import os

ZONE_INFO_DIR = "/usr/share/zoneinfo"

def parse_symlink():
  dirs = ("Africa", "America", "Antarctica", "Arctic", "Asia", "Atlantic",
          "Australia", "Brazil", "Canada", "Chile", "Etc", "Europe", "Indian",
          "Mexico", "Pacific")
  map = dict()
  for dir_name in dirs:
    for root, dirs, files in os.walk(os.path.join(ZONE_INFO_DIR, dir_name)):
      for file_name in files:
        abspath = os.path.join(root, file_name)
        if os.path.islink(abspath):
          real_path = os.path.realpath(abspath)
          if real_path not in map:
            map[real_path] = [real_path]
          map[real_path].append(abspath)
  return map

def read_zone_tab():
  result = list()
  with open("/usr/share/zoneinfo/zone.tab") as fh:
    for line in fh:
      if line and not line.startswith("#"):
        items = line.split("\t")
        if len(items) >= 3:
          result.append(os.path.join(ZONE_INFO_DIR, items[2].strip()))
  # Add Etc/UTC
  utc_list = ("Etc/UTC", "Etc/UCT", "Etc/GMT")
  result.extend(os.path.join(ZONE_INFO_DIR, name) for name in utc_list)
  return result

def merge_alias(alias_map, zones_map):
  result = []
  for alias_list in alias_map.values():
    real_name = ""
    for alias in alias_list:
      if alias in zones_map:
        real_name = alias
        break
    if real_name:
      for alias in alias_list:
        if alias != real_name:
          result.append((alias, real_name))
    else:
      print("Error:", alias_list)
  return result

def split_base_name(alias_list):
  pref = len(ZONE_INFO_DIR)
  return tuple((key[pref+1:], value[pref+1:]) for key, value in alias_list)

def print_result(alias_list):
  for alias_name, real_name in alias_list:
    print(alias_name, real_name, sep=':')

def main():
  symlinks = parse_symlink()
  zones = set(read_zone_tab())
  alias_list = merge_alias(symlinks, zones)
  alias_list = split_base_name(alias_list)
  print_result(alias_list)

if __name__ == "__main__":
  main()