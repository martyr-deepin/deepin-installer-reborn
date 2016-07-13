// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/system_language.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "application.h"
#include "base/file_util.h"
#include "service/settings_manager.h"

namespace service {

LanguageList GetLanguageList() {
  LanguageList result;

  const QString content =
      base::ReadTextFileContent(QStringLiteral(":/resources/languages.json"));
  const QJsonArray lang_list =
      QJsonDocument::fromJson(content.toUtf8()).array();
  for(const QJsonValue& lang_value : lang_list) {
    const QJsonObject obj = lang_value.toObject();
    LanguageItem item;
    item.locale = obj.value(QStringLiteral("locale")).toString();
    item.name = obj.value(QStringLiteral("name")).toString();
    item.local_name = obj.value(QStringLiteral("local_name")).toString();
    result.append(item);
  }

  return result;
}

void SetSystemLanguage(const QString& locale) {
  Q_UNUSED(locale);
  // TODO(xushaohua):
}

}  // namespace service