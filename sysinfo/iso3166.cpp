// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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