// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/timezone.h"

#include <QDebug>

#include "base/file_util.h"

namespace installer {

namespace {

// Absolute path to zone.tab file.
const char kZoneTabFile[] = "/usr/share/zoneinfo/zone.tab";

}  // namespace

//bool ZoneInfo::operator==(const ZoneInfo& other) {
//  return (this->country_code == other.country_code &&
//          this->timezone == other.timezone);
//}

QDebug& operator<<(QDebug& debug, const ZoneInfo& info) {
  debug << "ZoneInfo {"
        << "cc:" << info.country_code
        << "tz:" << info.timezone
        << "lat:" << info.latitude
        << "lon:" << info.longitude
        << "}";
  return debug;
}

ZoneInfoList GetZoneInfoList() {
  ZoneInfoList list;
  const QString content(ReadFile(kZoneTabFile));
  for (const QString& line : content.split('\n')) {
    if (!line.startsWith('#')) {
      const QStringList parts(line.split('\t'));
      if (parts.length() >= 3) {
        const QString coordinates = parts.at(1);
        int longitude_index = coordinates.indexOf('+', 3);
        if (longitude_index == -1) {
          longitude_index = coordinates.indexOf('-', 3);
        }
        Q_ASSERT(longitude_index > -1);
        const int latitude = coordinates.left(longitude_index).toInt();
        const int longitude = coordinates.mid(longitude_index).toInt();
        const ZoneInfo zone_info = {parts.at(0), parts.at(2),
                                    latitude, longitude};
        list.append(zone_info);
      }
    }
  }
  return list;
}

int GetZoneInfoByCountryCode(const ZoneInfoList& list,
                             const QString& country_code) {
  int index = -1;
  for (const ZoneInfo& info : list) {
    index ++;
    if (info.country_code == country_code) {
      return index;
    }
  }
  return -1;
}

int GetZoneInfoByTimezone(const ZoneInfoList& list, const QString& timezone) {
  int index = -1;
  for (const ZoneInfo& info : list) {
    index ++;
    if (info.timezone == timezone) {
      return index;
    }
  }
  return -1;
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
  const int index = timezone.lastIndexOf('/');
  return (index > -1) ? timezone.mid(index + 1) : timezone;
}

bool IsValidTimezone(const QString& timezone) {
  // Ignores empty timezone.
  if (timezone.isEmpty()) {
    return false;
  }

  const ZoneInfoList list = GetZoneInfoList();
  return (GetZoneInfoByTimezone(list, timezone) > -1);
}

}  // namespace installer