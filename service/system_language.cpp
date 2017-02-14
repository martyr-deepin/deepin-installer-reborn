// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/system_language.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

namespace {

// Absolute path to locale.gen
const char kLocaleGenFile[] = "/etc/locale.gen";

}  // namespace

LanguageList GetLanguageList() {
  LanguageList list;

  const QString content(ReadFile(RESOURCES_DIR "/languages.json"));
  const QJsonArray lang_list =
      QJsonDocument::fromJson(content.toUtf8()).array();
  for (const QJsonValue& lang_value : lang_list) {
    const QJsonObject obj = lang_value.toObject();
    LanguageItem item;
    item.name = obj.value("name").toString();
    item.locale = obj.value("locale").toString();
    item.lc_all = obj.value("lc_all").toString();
    item.local_name = obj.value("local_name").toString();
    list.append(item);
  }

  return list;
}

void GenerateLocale(const QString& lc_all) {
  const QString content = ReadFile(kLocaleGenFile);
  QStringList lines = content.split('\n');
  for (QString& line : lines) {
    // Match locale name with case-insensitive.
    if (line.contains(lc_all, Qt::CaseInsensitive)) {
      line = line.remove('#').trimmed();
      break;
    }
  }
  const QString result = lines.join('\n');
  if (!WriteTextFile(kLocaleGenFile, result)) {
    qCritical() << "Failed to update locale.gen file";
    return;
  }

  QString out, err;
  if (!SpawnCmd("locale-gen", {}, out, err)) {
    qCritical() << "locale-gen failed:" << out << err;
  }

  // Update default locale.
  const QString default_locale = QString("LANG=%1").arg(lc_all);
  if (!WriteTextFile("/etc/default/locale", default_locale)) {
    qCritical() << "Failed to update default locale";
  }
}

}  // namespace installer
