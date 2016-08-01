// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_SETTINGS_NAME_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_SETTINGS_NAME_H

namespace service {

// Keys used in settings ini file.

// Pages
const char kSkipDiskSpaceInsufficientPageName[] =
    "skip_disk_space_insufficient_page";
const char kSkipVirtualMachinePageName[] = "skip_virtual_machine_page";
const char kSkipSelectLanguagePageName[] = "skip_select_language_page";
const char kSkipSystemInfoPageName[] = "skip_system_info_page";
const char kSkipPartitionPageName[] = "skip_partition_page";

// System Language List
const char kSelectLanguageUseDefaultLocaleName[] =
    "select_language_use_default_lcoale";
const char kSelectLanguageDefaultLocaleName[] =
    "select_language_default_locale";

// System Info
const char kSystemInfoDdeDefaultWallpaperName[] =
    "system_info_dde_default_wallpaper";
const char kSystemInfoUseOemWallpaperName[] = "system_info_use_oem_wallpaper";
const char kSystemInfoUseVendorLogoName[] = "system_info_use_vendor_logo";
const char kSystemInfoSetupAfterRebootName[] = "system_info_setup_after_reboot";
const char kSystemInfoUseDefaultUsernameName[] =
    "system_info_use_default_username";
const char kSystemInfoDefaultUsernameName[] = "system_info_default_username";
const char kSystemInfoUseDefaultPasswordName[] =
    "system_info_use_default_password";
const char kSystemInfoDefaultPasswordName[] = "system_info_default_password";
const char kSystemInfoUseDefaultHostnameName[] =
    "system_info_use_default_hostname";
const char kSystemInfoDefaultHostnameName[] = "system_info_default_hostname";
const char kSystemInfoUseRandomAvatorName[] = "system_info_use_random_avator";
const char kSystemInfoUseDefaultAvatorName[] = "system_info_use_default_avatar";
const char kSystemInfoDefaultAvatorName[] = "system_info_default_avatar";
const char kSystemInfoDdeAvatarDirName[] = "system_info_dde_avatar_dir";
const char kSystemInfoDisableAvatorPageName[] =
    "system_info_disable_avatar_page";
const char kSystemInfoDisableTimezonePageName[] =
    "system_info_disable_timezone_page";

// Partition
const char kPartitionMinimumDiskSpaceRequiredName[] =
    "partition_minimum_disk_space_required";
const char kPartitionRecommendedDiskSpaceName[] =
    "partition_recommended_disk_space";
const char kPartitionDoAutoPartName[] = "partition_do_auto_part";


// Install failed page
const char kInstallFailedFeedbackServerName[] =
    "install_failed_feedback_server";

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_SETTINGS_NAME_H
