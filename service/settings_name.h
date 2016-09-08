// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_SETTINGS_NAME_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_SETTINGS_NAME_H

namespace service {

// Keys used in settings ini file.

// Pages
const char kSkipDiskSpaceInsufficientPage[] =
    "skip_disk_space_insufficient_page";
const char kSkipVirtualMachinePage[] = "skip_virtual_machine_page";
const char kSkipSelectLanguagePage[] = "skip_select_language_page";
const char kSkipSystemInfoPage[] = "skip_system_info_page";
const char kSkipPartitionPage[] = "skip_partition_page";

// System Language List
const char kSelectLanguageUseDefaultLocale[] =
    "select_language_use_default_lcoale";
const char kSelectLanguageDefaultLocale[] = "select_language_default_locale";

// System Info
const char kSystemInfoDdeDefaultWallpaper[] =
    "system_info_dde_default_wallpaper";
const char kSystemInfoSetupAfterReboot[] = "system_info_setup_after_reboot";
const char kSystemInfoUseDefaultUsername[] =
    "system_info_use_default_username";
const char kSystemInfoDefaultUsername[] = "system_info_default_username";
const char kSystemInfoUseDefaultPasswordName[] =
    "system_info_use_default_password";
const char kSystemInfoDefaultPassword[] = "system_info_default_password";
const char kSystemInfoPasswordMinLength[] = "system_info_password_min_length";
const char kSystemInfoPasswordMaxLength[] = "system_info_password_max_length";
const char kSystemInfoPasswordShallContainNumber[] =
    "system_info_password_shall_contain_number";
const char kSystemInfoPasswordShallContainLowerCase[] =
    "system_info_password_shall_contain_lower_case";
const char kSystemInfoPasswordShallContainUpperCase[] =
    "system_info_password_shall_contain_upper_case";
const char kSystemInfoPasswordShallContainSpecialChar[] =
    "system_info_password_shall_contain_special_char";
const char kSystemInfoUseDefaultHostname[] =
    "system_info_use_default_hostname";
const char kSystemInfoDefaultHostname[] = "system_info_default_hostname";
const char kSystemInfoUseRandomAvator[] = "system_info_use_random_avator";
const char kSystemInfoUseDefaultAvator[] = "system_info_use_default_avatar";
const char kSystemInfoDefaultAvator[] = "system_info_default_avatar";
const char kSystemInfoDdeAvatarDir[] = "system_info_dde_avatar_dir";
const char kSystemInfoUseDefaultTimezone[] =
    "system_info_use_default_timezone";
const char kSystemInfoDefaultTimezone[] = "system_info_default_timezone";
const char kSystemInfoDisableAvatorPage[] = "system_info_disable_avatar_page";
const char kSystemInfoDisableTimezonePage[] =
    "system_info_disable_timezone_page";

// Partition
const char kPartitionMinimumDiskSpaceRequired[] =
    "partition_minimum_disk_space_required";
const char kPartitionRecommendedDiskSpace[] =
    "partition_recommended_disk_space";
const char kPartitionSupportedFs[] = "partition_supported_fs";
const char kPartitionDoAutoPart[] = "partition_do_auto_part";
const char kPartitionMountPoints[] = "partition_mount_points";

// Install failed page
const char kInstallFailedFeedbackServer[] = "install_failed_feedback_server";

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_SETTINGS_NAME_H
