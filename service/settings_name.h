// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_SETTINGS_NAME_H
#define INSTALLER_SERVICE_SETTINGS_NAME_H

namespace installer {

// Keys used in settings ini file.

// Pages
const char kSkipDiskSpaceInsufficientPage[] =
    "skip_disk_space_insufficient_page";
const char kSkipVirtualMachinePage[] = "skip_virtual_machine_page";
const char kSkipSelectLanguagePage[] = "skip_select_language_page";
const char kSkipSystemInfoPage[] = "skip_system_info_page";
const char kSkipPartitionPage[] = "skip_partition_page";

// System Language List
const char kSelectLanguageDefaultLocale[] = "select_language_default_locale";

// System Info
const char kSystemInfoDeepinVersion[] = "system_info_deepin_version";
const char kSystemInfoLsbRelease[] = "system_info_lsb_release";
const char kSystemInfoVendorName[] = "system_info_vendor_name";
const char kSystemInfoSetupAfterReboot[] = "system_info_setup_after_reboot";
const char kSystemInfoDefaultUsername[] = "system_info_default_username";
const char kSystemInfoLockUsername[] = "system_info_lock_username";
const char kSystemInfoUsernameMinLen[] = "system_info_username_min_len";
const char kSystemInfoUsernameMaxLen[] = "system_info_username_max_len";
const char kSystemInfoDefaultHostname[] = "system_info_default_hostname";
const char kSystemInfoLockHostname[] = "system_info_lock_hostname";
const char kSystemInfoHostnameAutoSuffix[] = "system_info_hostname_auto_suffix";
const char kSystemInfoHostnameMinLen[] = "system_info_hostname_min_len";
const char kSystemInfoHostnameMaxLen[] = "system_info_hostname_max_len";
const char kSystemInfoHostnameReserved[] = "system_info_hostname_reserved";
const char kSystemInfoDefaultPassword[] = "system_info_default_password";
const char kSystemInfoLockPassword[] = "system_info_lock_password";
const char kSystemInfoPasswordMinLen[] = "system_info_password_min_len";
const char kSystemInfoPasswordMaxLen[] = "system_info_password_max_len";
const char kSystemInfoPasswordRequireNumber[] =
    "system_info_password_require_number";
const char kSystemInfoPasswordRequireLowerCase[] =
    "system_info_password_require_lower_case";
const char kSystemInfoPasswordRequireUpperCase[] =
    "system_info_password_require_upper_case";
const char kSystemInfoPasswordRequireSpecialChar[] =
    "system_info_password_require_special_char";
const char kSystemInfoDefaultAvator[] = "system_info_default_avatar";
const char kSystemInfoDdeAvatarDir[] = "system_info_dde_avatar_dir";
const char kSystemInfoDisableAvatorPage[] = "system_info_disable_avatar_page";

const char kSystemInfoDefaultKeyboardLayout[] =
    "system_info_default_keyboard_layout";
const char kSystemInfoDefaultKeyboardLayoutVariant[] =
    "system_info_default_keyboard_layout_variant";
const char kSystemInfoDisableKeyboardPage[] =
    "system_info_disable_keyboard_page";

const char kSystemInfoDefaultTimezone[] = "system_info_default_timezone";
const char kSystemInfoUseWindowsTime[] = "system_info_use_windows_time";
const char kSystemInfoDisableTimezonePage[] =
    "system_info_disable_timezone_page";
const char kSystemInfoWindowsDisableTimezonePage[] =
    "system_info_windows_disable_timezone_page";
const char kSystemInfoTimezoneUseGeoIp[] = "system_info_timezone_use_geoip";
const char kSystemInfoTimezoneUseRegdomain[] =
    "system_info_timezone_use_regdomain";

// Partition
const char kPartitionSkipSimplePartitionPage[] =
    "partition_skip_simple_partition_page";
const char kPartitionEnableSimpleDiskPage[] =
    "partition_enable_simple_disk_page";
const char kPartitionSkipAdvancedPartitionPage[] =
    "partition_skip_advanced_partition_page";
const char kPartitionEnableSwapFile[] = "partition_enable_swap_file";
const char kPartitionEnableSwapFileInAdvancedPage[] =
    "partition_enable_swap_file_in_advanced_page";
const char kPartitionMemoryThresholdForSwapArea[] =
    "partition_memory_threshold_for_swap_area";
const char kPartitionSwapPartitionSize[] = "partition_swap_partition_size";
const char kPartitionMinimumDiskSpaceRequired[] =
    "partition_minimum_disk_space_required";
const char kPartitionRecommendedDiskSpace[] =
    "partition_recommended_disk_space";
const char kPartitionDefaultBootSpace[] = "partition_default_boot_space";
const char kPartitionDefaultEFISpace[] = "partition_default_efi_space";
const char kPartitionEFIMinimumSpace[] = "partition_efi_minimum_space";
const char kPartitionSupportedFs[] = "partition_supported_fs";
const char kPartitionDefaultFs[] = "partition_default_fs";
const char kPartitionDoAutoPart[] = "partition_do_auto_part";
const char kPartitionMountPoints[] = "partition_mount_points";
const char kPartitionFormattedMountPoints[] =
    "partition_formatted_mount_points";
const char kPartitionBootOnFirstPartition[] =
    "partition_boot_on_first_partition";
const char kPartitionPreferLogicalPartition[] =
    "partition_prefer_logical_partition";
const char kPartitionBootPartitionFs[] = "partition_boot_partition_fs";
const char kPartitionEnableOsProber[] = "partition_enable_os_prober";

// Install progress page
const char kInstallProgressPageDisableSlide[] =
    "install_progress_page_disable_slide";
const char kInstallProgressPageDisableSlideAnimation[] =
    "install_progress_page_disable_slide_animation";
const char kInstallProgressPageAnimationDuration[] =
    "install_progress_page_animation_duration";

// Install failed page
const char kInstallFailedFeedbackServer[] = "install_failed_feedback_server";
const char kInstallFailedQRErrMsgLen[] = "install_failed_qr_err_msg_len";
const char kInstallFailedErrMsgLen[] = "install_failed_err_msg_len";

// Packages
const char kPackageUninstalledPackages[] = "package_uninstalled_packages";

// Grub
const char kGrubTimeout[] = "grub_timeout";
const char kGrubBlockWindows[] = "grub_block_windows";

// Desktop
const char kDdeDockAppList[] = "dde_dock_app_list";
const char kDdeLauncherAppList[] = "dde_launcher_app_list";
const char kDdeLauncherHoldPackages[] = "dde_launcher_hold_packages";
const char kDdeDesktopDesktopContent[] = "dde_desktop_desktop_content";
const char kDdeSoundEffectSwitches[] = "dde_sound_effect_switches";

// Service
const char kServiceEnabledServices[] = "service_enabled_services";
const char kServiceDisabledServices[] = "service_disabled_services";

}  // namespace installer

#endif  // INSTALLER_SERVICE_SETTINGS_NAME_H
