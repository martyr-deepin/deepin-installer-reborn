// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H

#include <QString>
#include <QStringList>
#include <QVariant>

namespace service {

// Read settings value from ini file.

// Get boolean option value from settings file.
// If |key| does not exist, returns false.
bool GetSettingsBool(const QString& key);

// Get string option value from settings file.
// If |key| does not exist, returns an empty string.
QString GetSettingsString(const QString& key);

// Get variant option value from settings file.
QVariant GetSettingsValue(const QString& key);

// Returns absolute path to oem/auto-part.sh
QString GetAutoPartFile();

// Get all available avatars in avatar folder.
QStringList GetAvatars();

// Get default user avatar, is specified. Or else returns a random avatar.
QString GetDefaultAvatar();

// Get absolute path to oem hooks folder.
QString GetOemHooksDir();

// Get folder of overlay module.
QString GetOverlayFilesystemDir();

// Get vendor logo.
QString GetVendorLogo();

// Get image path of window background.
// First try to find in root of iso.
// Then read from /usr/share/backgrounds/.
// If not appropriate image is found, use the fallback image.
QString GetWindowBackground();

// Operations of /etc/deepin-installer.conf
bool DeleteConfigFile();
//void WriteBootloader(const QString& path);
//void WriteUEFI(bool is_efi);
//void WriteInstallerMode(bool is_simple_mode);
void WriteLocale(const QString& locale);
void WriteUsername(const QString& username);
void WriteHostname(const QString& hostname);
void WritePassword(const QString& password);
void WriteAvatar(const QString& avatar);
void WriteTimezone(const QString& timezone);
void WriteKeyboard(const QString& layout, const QString& variant);
void WritePartitionInfo(const QString& root, const QString& mount_points);

// Save current oem settings to /etc/deepin-installer-oem.conf
// This file is accessed in hook scripts.
void SaveOemConfig();

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H
