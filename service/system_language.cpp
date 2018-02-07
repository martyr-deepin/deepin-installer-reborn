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

#include "service/system_language.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

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
    item.local_name = obj.value("local_name").toString();
    item.timezone = obj.value("timezone").toString();
    list.append(item);
  }

  return list;
}

void GenerateLocale(const QString& locale) {
  // Absolute path to locale.gen
  const char kLocaleGenFile[] = "/etc/locale.gen";
  const QString content = ReadFile(kLocaleGenFile);
  QStringList lines = content.split('\n');
  for (QString& line : lines) {
    // Match locale name with case-insensitive.
    if (line.contains(locale, Qt::CaseInsensitive)) {
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
  const QString default_locale = QString("LANG=%1").arg(locale);
  if (!WriteTextFile("/etc/default/locale", default_locale)) {
    qCritical() << "Failed to update default locale";
  }
}

}  // namespace installer
