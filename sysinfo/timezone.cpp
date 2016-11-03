// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/timezone.h"

#include <QDebug>

#include "base/file_util.h"

namespace installer {

namespace {

const char kZoneTabFile[] = "/usr/share/zoneinfo/zone.tab";

}  // namespace

ZoneInfo::ZoneInfo(const QString& country_code, const QString& timezone)
    : country_code(country_code),
      timezone(timezone) {
}

bool ZoneInfo::operator==(const ZoneInfo& other) {
  return (this->country_code == other.country_code &&
          this->timezone == other.timezone);
}

ZoneInfoList GetZoneInfoList() {
  ZoneInfoList list;
  const QString content(ReadFile(kZoneTabFile));
  for (const QString& line : content.split('\n')) {
    if (!line.startsWith('#')) {
      const QStringList parts = line.split('\t');
      if (parts.length() >= 3) {
        ZoneInfo zone_info(parts[0], parts[2]);
        list.append(zone_info);
      }
    }
  }
  return list;
}

QString GetCurrentTimezone() {
  const QString content(ReadFile("/etc/timezone"));
  return content.trimmed();
}

QString GetPreferTimezone() {
  // TODO(xushaohua): Read locale info.
  return "UTC";
}

QString GetTimezoneName(const QString& timezone) {
  const int index = timezone.indexOf('/');
  return (index > -1) ? timezone.mid(index + 1) : timezone;
}

bool IsValidTimezone(const QString& timezone) {
  // Ignores empty timezone.
  if (timezone.isEmpty()) {
    return false;
  }

  const ZoneInfoList list = GetZoneInfoList();
  for (const ZoneInfo& zone_info : list) {
    if (zone_info.timezone == timezone) {
      return true;
    }
  }
  return false;
}

}  // namespace installer