// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_TIMEZONE_H
#define INSTALLER_SYSINFO_TIMEZONE_H

#include <QList>

namespace installer {

struct ZoneInfo {
 public:
  ZoneInfo(const QString& country_code, const QString& timezone);

  bool operator==(const ZoneInfo& other);

  QString country_code;
  QString timezone;
};
typedef QList<ZoneInfo> ZoneInfoList;

// Read available timezone info in zone.tab file.
ZoneInfoList GetZoneInfoList();

// Read available timezone info in zone.tab file.
QStringList GetZoneInfo();

// Read current timezone in /etc/timezone file.
QString GetCurrentTimezone();

// Get prefer timezone based on locale.
QString GetPreferTimezone();

// Returns name of timezone, excluding continent name.
QString GetTimezoneName(const QString& timezone);

// Validate |timezone|.
// TODO(xushaohua): Optimize
bool IsValidTimezone(const QString& timezone);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_TIMEZONE_H
