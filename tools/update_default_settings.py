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

# Generate default settings for loongson and sw platforms.
# Execute this program after updating options in resources/default_settings.ini.

import configparser
import os
import shutil
import sys

SEC_NAME = "General"

def update_settings(settings_file, settings):
    src_settings = "resources/default_settings.ini"
    if not os.path.exists(src_settings):
        print("Failed to find", src_settings)
        sys.exit(1)
    shutil.copy(src_settings, settings_file)

    parser = configparser.RawConfigParser()
    parser.read(settings_file)
    for key, value in settings:
        parser.set(SEC_NAME, key, value)
    with open(settings_file, "w") as fh:
        parser.write(fh)

def main():
    arm_file = "resources/arm_default_settings.ini"
    loongson_file = "resources/loongson_default_settings.ini"
    sw_file = "resources/sw_default_settings.ini"
    professional_file = "resources/professional_default_settings.ini"

    arm_settings = (
            ("skip_virtual_machine_page", "true"),
            ("apt_source_deb", '"deb http://packages.deepin.com/deepin camel main contrib non-free"'),
            ("apt_source_deb_src", '"#deb-src http://packages.deepin.com/deepin camel main contrib non-free"'),
            ("partition_skip_partition_crypt_page", "true")
    )

    loongson_settings = (
            ("skip_virtual_machine_page", "true"),
            ("skip_timezone_page", "true"),
            ("select_language_default_locale", "zh_CN"),
            ("system_info_disable_keyboard_page", "true"),
            ("system_info_default_keyboard_layout", '"us"'),
            ("timezone_default", '"Asia/Shanghai"'),
            ("timezone_use_regdomain", "false"),
            ("partition_skip_simple_partition_page", "true"),
            ("partition_skip_partition_crypt_page", "true"),
            ("partition_enable_simple_disk_page", "true"),
            ("partition_enable_swap_file", "false"),
            ("partition_formatted_mount_points", '"/;/tmp;/var"'),
            ("partition_enable_os_prober", "false"),
            ("partition_boot_on_first_partition", "true"),
            ("partition_supported_fs", '"ext4;ext3;ext2;efi;linux-swap"'),
            ("partition_prefer_logical_partition", "false"),
            ("partition_full_disk_small_legacy_policy", '"/boot:ext4:1:1536;swap:linux-swap:1537:swap-size;/:ext4::100%"'),
            ("partition_full_disk_large_legacy_policy", '"/boot:ext4:1:1536;swap:linux-swap:1537:swap-size;/:ext4::75%;:ext4::100%"'),
            ("partition_full_disk_small_legacy_label", '"Boot;Swap;Root"'),
            ("partition_full_disk_large_legacy_label", '"Boot;Swap;Root;_dde_data"'),
            ("apt_source_deb", '"deb http://packages.deepin.com/deepin camel main contrib non-free"'),
            ("apt_source_deb_src", '"#deb-src http://packages.deepin.com/deepin camel main contrib non-free"'),
    )

    sw_settings = (
        ("skip_virtual_machine_page", "true"),
        ("skip_timezone_page", "true"),
        ("select_language_default_locale", "zh_CN"),
        ("system_info_disable_keyboard_page", "true"),
        ("system_info_default_keyboard_layout", '"us"'),
        ("timezone_default", '"Asia/Shanghai"'),
        ("timezone_use_regdomain", "false"),
        ("partition_skip_simple_partition_page", "true"),
        ("partition_skip_partition_crypt_page", "true"),
        ("partition_enable_swap_file", "false"),
        ("partition_swap_partition_size", "8192"),
        ("partition_enable_simple_disk_page", "true"),
        ("partition_enable_swap_file", "false"),
        ("partition_formatted_mount_points", '"/;/tmp;/var"'),
        ("partition_enable_os_prober", "false"),
        ("partition_boot_on_first_partition", "true"),
        ("partition_supported_fs", '"ext4;ext3;ext2;efi;linux-swap"'),
        ("partition_prefer_logical_partition", "false"),
        ("partition_full_disk_small_legacy_policy", '"/boot:ext4:1:3072;swap:linux-swap:3073:swap-size;/:ext4::100%"'),
        ("partition_full_disk_large_legacy_policy", '"/boot:ext4:1:3072;swap:linux-swap:3073:swap-size;/:ext4::75%;:ext4::100%"'),
        ("partition_full_disk_small_legacy_label", '"Boot;Swap;Root"'),
        ("partition_full_disk_large_legacy_label", '"Boot;Swap;Root;_dde_data"'),
        ("apt_source_deb", '"deb http://packages.deepin.com/deepin camel main contrib non-free"'),
        ("apt_source_deb_src", '""'),
    )

    professinal_settings = (
        ("timezone_use_local_time_regardless", "true"),
        ("system_info_password_strong_check", "false"),
        ("apt_source_deb", '"deb http://packages.deepin.com/deepin camel main contrib non-free"'),
        ("apt_source_deb_src", '"#deb-src http://packages.deepin.com/deepin camel main contrib non-free"'),
    )

    update_settings(arm_file, arm_settings)
    update_settings(loongson_file, loongson_settings)
    update_settings(sw_file, sw_settings)
    update_settings(professional_file, professinal_settings)

if __name__ == "__main__":
    main()
