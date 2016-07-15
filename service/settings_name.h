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

// System Language List
const char kSelectLanguageUseDefaultLocaleName[] =
    "select_language_use_default_lcoale";
const char kSelectLanguageDefaultLocaleName[] =
    "select_language_default_locale";

// System Info
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

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_SETTINGS_NAME_H
