#!/usr/bin/env python3
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Generate default settings for loongson and sw platforms.

import configparser
import os
import shutil
import sys

SEC_NAME = "General"
def update_loongson_policy(settings_file):
    parser = configparser.ConfigParser()
    parser.read(settings_file)
    parser.set(SEC_NAME, "skip_virtual_machine_page", "true")
    parser.set(SEC_NAME, "system_info_default_timezone", "Asia/Shanghai")
    parser.set(SEC_NAME, "system_info_timezone_use_regdomain", "false")
    parser.set(SEC_NAME, "partition_skip_simple_partition_page", "true")
    parser.set(SEC_NAME, "partition_formatted_mount_points", "/;/tmp;/usr;/var")
    parser.set(SEC_NAME, "partition_enable_os_prober", "false")
    parser.set(SEC_NAME, "partition_boot_on_first_partition", "true")
    with open(settings_file, "w") as fh:
        parser.write(fh)

def update_sw_policy(settings_file):
    parser = configparser.ConfigParser()
    parser.read(settings_file)
    parser.set(SEC_NAME, "skip_virtual_machine_page", "true")
    parser.set(SEC_NAME, "system_info_default_timezone", "Asia/Shanghai")
    parser.set(SEC_NAME, "system_info_timezone_use_regdomain", "false")
    parser.set(SEC_NAME, "partition_skip_simple_partition_page", "true")
    parser.set(SEC_NAME, "partition_formatted_mount_points", "/;/tmp;/usr;/var")
    parser.set(SEC_NAME, "partition_enable_os_prober", "false")
    parser.set(SEC_NAME, "partition_boot_on_first_partition", "true")
    with open(settings_file, "w") as fh:
        parser.write(fh)

def main():
    src_settings = "resources/default_settings.ini"
    loongson_settings = "resources/loongson_default_settings.ini"
    sw_settings = "resources/sw_default_settings.ini"
    if not os.path.exists(src_settings):
        print("Failed to find", src_settings)
        sys.exit(1)
    shutil.copy(src_settings, loongson_settings)
    shutil.copy(src_settings, sw_settings)

    update_loongson_policy(loongson_settings)
    update_sw_policy(sw_settings)

if __name__ == "__main__":
    main()
