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

// Get all available avatars in avatar folder.
QStringList GetAvatars();

// Get default user avatar, is specified. Or else returns a random avatar.
QString GetDefaultAvatar();

// Get absolute path to oem hooks folder.
QString GetOemHooksDir();

// Get vendor logo.
QString GetVendorLogo();

// Get image path of window background.
// First try to find in root of iso.
// Then read from /usr/share/backgrounds/.
// If not appropriate image is found, use the fallback image.
QString GetWindowBackground();

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H
