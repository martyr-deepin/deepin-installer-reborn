// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_TIMEZONE_H
#define INSTALLER_SYSINFO_TIMEZONE_H

#include <QList>

namespace installer {

struct ZoneInfo {
 public:
  QString country_code;
  QString timezone;
  // Coordinates of zone.
  int latitude;
  int longitude;
};
QDebug& operator<<(QDebug& debug, const ZoneInfo& info);
typedef QList<ZoneInfo> ZoneInfoList;

// Read available timezone info in zone.tab file.
ZoneInfoList GetZoneInfoList();

// Find ZoneInfo based on |country_code| or |timezone|.
// Returns -1 if not found.
int GetZoneInfoByCountryCode(const ZoneInfoList& list,
                             const QString& country_code);
int GetZoneInfoByTimezone(const ZoneInfoList& list,
                          const QString& timezone);

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
