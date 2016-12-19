#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

import os


def parse_symlink():
  dirs = ("Africa", "America", "Antarctica", "Arctic", "Asia", "Atlantic",
          "Australia", "Brazil", "Canada", "Chile", "Etc", "Europe", "Indian",
          "Mexico", "Pacific")
  parent_dir = "/usr/share/zoneinfo"
  parent_len = len(parent_dir)
  result = list()
  for dir_name in dirs:
    dir_path = os.path.join(parent_dir, dir_name)
    for entry in os.listdir(dir_path):
      abspath = os.path.join(dir_path, entry)
      if os.path.isdir(abspath):
        pass
      elif os.path.islink(abspath):
        tz_name = dir_name + "/" + entry
        real_path = os.path.realpath(abspath)[parent_len+1:]
        result.append((tz_name, real_path))
  return result

def read_zone_tab():
  result = list()
  with open("/usr/share/zoneinfo/zone.tab") as fh:
    for line in fh:
      if line and not line.startswith("#"):
        items = line.split("\t")
        if len(items) >= 3:
          result.append(items[2].strip())
  return result

def main():
  symlinks = parse_symlink()
  zones = set(read_zone_tab())
  symlink_map = dict()
  for tz_name, real_name in symlinks:
    if real_name not in symlink_map:
      symlink_map[real_name] = list()
    symlink_map[real_name].append(tz_name)

  result = dict()
  for real_name in symlink_map:
    alias_list = symlink_map[real_name]
    real_name2 = ""
    if real_name in zones:
      real_name2 = real_name
    else:
      for alias in alias_list:
        if alias in zones:
          real_name2 = alias
          break
    for alias in alias_list:
      if alias != real_name2:
        result[alias] = real_name2
  for item in result:
    print(item, result[item])

if __name__ == "__main__":
  main()