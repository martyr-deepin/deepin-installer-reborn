// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/timezone.h"

#include <cmath>
#include <QDebug>

#include "base/file_util.h"

namespace installer {

namespace {

// Absolute path to zone.tab file.
const char kZoneTabFile[] = "/usr/share/zoneinfo/zone.tab";

// Absolute path to backward timezone file.
const char kBackwardTzFile[] = RESOURCES_DIR "backward_tz";

// Parse latitude and longitude of the zone's principal location.
// See https://en.wikipedia.org/wiki/List_of_tz_database_time_zones.
// |pos| is in ISO 6709 sign-degrees-minutes-seconds format,
// either +-DDMM+-DDDMM or +-DDMMSS+-DDDMMSS.
// |digits| 2 for latitude, 3 for longitude.
double ConvertPos(const QString& pos, int digits) {
  if (pos.length() < 4 || digits > 9) {
    return 0.0;
  }

  const QString integer = pos.left(digits + 1);
  const QString fraction = pos.mid(digits + 1);
  const double t1 = integer.toDouble();
  const double t2 = fraction.toDouble();
  if (t1 > 0.0) {
    return t1 + t2 / pow(10.0, fraction.length());
  } else {
    return t1 - t2 / pow(10.0, fraction.length());
  }
}

}  // namespace

bool ZoneInfoDistanceComp(const ZoneInfo& a, const ZoneInfo& b) {
  return a.distance < b.distance;
}

QDebug& operator<<(QDebug& debug, const ZoneInfo& info) {
  debug << "ZoneInfo {"
        << "cc:" << info.country
        << "tz:" << info.timezone
        << "lat:" << info.latitude
        << "lng:" << info.longitude
        << "}";
  return debug;
}

ZoneInfoList GetZoneInfoList() {
  ZoneInfoList list;
  const QString content(ReadFile(kZoneTabFile));
  for (const QString& line : content.split('\n')) {
    if (!line.startsWith('#')) {
      const QStringList parts(line.split('\t'));
      // Parse latitude and longitude.
      if (parts.length() >= 3) {
        const QString coordinates = parts.at(1);
        int index = coordinates.indexOf('+', 3);
        if (index == -1) {
          index = coordinates.indexOf('-', 3);
        }
        Q_ASSERT(index > -1);
        const double latitude = ConvertPos(coordinates.left(index), 2);
        const double longitude = ConvertPos(coordinates.mid(index), 3);
        const ZoneInfo zone_info = {parts.at(0), parts.at(2),
                                    latitude, longitude, 0.0};
        list.append(zone_info);
      }
    }
  }
  return list;
}

int GetZoneInfoByCountry(const ZoneInfoList& list,
                         const QString& country) {
  int index = -1;
  for (const ZoneInfo& info : list) {
    index ++;
    if (info.country == country) {
      return index;
    }
  }
  return -1;
}

int GetZoneInfoByZone(const ZoneInfoList& list, const QString& timezone) {
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

BackwardTzMap GetBackwardTzMap() {
  BackwardTzMap map;

  const QString content = ReadFile(kBackwardTzFile);
  for (const QString& line : content.split('\n')) {
    // Ignores empty line and comment.
    if (line.startsWith("Link\t")) {
      const QStringList parts(line.split('\t'));
      Q_ASSERT(parts.length() == 3);
      if (parts.length() == 3) {
        QString real = parts.at(1);
        if ((real == "Etc/UTC") || (real == "Etc/UCT")) {
          real = "Etc/GMT";
        }
        map.insert(real, parts.at(2));
      }
    }
  }

  return map;
}

bool IsValidTimezone(const QString& timezone) {
  // Ignores empty timezone.
  if (timezone.isEmpty()) {
    return false;
  }

  const ZoneInfoList list = GetZoneInfoList();
  return (GetZoneInfoByZone(list, timezone) > -1);
}

}  // namespace installer