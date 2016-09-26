// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SYSINFO_TIMEZONE_H
#define DEEPIN_INSTALLER_REBORN_SYSINFO_TIMEZONE_H

#include <QStringList>

namespace sysinfo {

// Read available timezone info in zone.tab file.
QStringList GetZoneInfo();

// Read current timezone in /etc/timezone file.
QString GetCurrentTimezone();

// Get prefer timezone based on locale.
QString GetPreferTimezone();

// Returns name of timezone, excluding continent name.
QString GetTimezoneName(const QString& timezone);

// Validate |timezone|.
bool IsValidTimezone(const QString& timezone);

}  // namespace sysinfo

#endif  // DEEPIN_INSTALLER_REBORN_SYSINFO_TIMEZONE_H
