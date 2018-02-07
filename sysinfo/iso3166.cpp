/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sysinfo/iso3166.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/file_util.h"

namespace installer {

namespace {

const char kIsoStandardFile[] = "/usr/share/iso-codes/json/iso_3166-1.json";

}  // namespace installer

CountryEntries GetCountryEntries() {
  CountryEntries entries;
  QByteArray content;

  if (ReadRawFile(kIsoStandardFile, content)) {
    const QJsonDocument doc(QJsonDocument::fromJson(content));
    const QJsonObject obj(doc.object());
    if (obj.contains("3166-1")) {
      const QJsonValue root = obj.value("3166-1");
      if (root.isArray()) {
        const QJsonArray array = root.toArray();
        for (const QJsonValue& value : array) {
          if (value.isObject()) {
            const QJsonObject val = value.toObject();
            CountryEntry entry;
            entry.alpha2 = val.value("alpha_2").toString();
            entry.alpha3 = val.value("alpha_3").toString();
            entry.name = val.value("name").toString();
            entry.official_name = val.value("official_name").toString();
            entry.numberic = val.value("numeric").toInt();
            entries.append(entry);
          }
        }
      }
    }
  }

  return entries;
}

int GetCountryEntryIndex(const CountryEntries& entries, const QString& cc) {
  for (int i = 0; i < entries.length(); i++) {
    if (entries.at(i).alpha2 == cc || entries.at(i).alpha3 == cc) {
      return i;
    }
  }
  return -1;
}

}  // namespace installer