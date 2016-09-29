// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/timezone.h"

#include <QDebug>
#include <QTextStream>

#include "base/file_util.h"

namespace installer {

QStringList GetZoneInfo() {
  QStringList result;
  QFile file(QStringLiteral("/usr/share/zoneinfo/zone.tab"));
  if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
    qWarning() << "GetZoneInfo() Failed to open zone.tab!";
    return result;
  }

  QTextStream stream(&file);
  while (!stream.atEnd()) {
    QString line = stream.readLine();
    if (line.startsWith('#')) {
      continue;
    }
    const QStringList line_parts = line.split(QChar('\t'));
    if (line_parts.length() > 3) {
      result.append(line_parts[2]);
    }
  }

  file.close();
  return result;
}

QString GetCurrentTimezone() {
  const QString content = ReadTextFileContent("/etc/timezone");
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
  const QStringList timezones = GetZoneInfo();
  return timezones.contains(timezone);
}

}  // namespace installer