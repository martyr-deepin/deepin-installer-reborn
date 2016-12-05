// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/system_language.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/file_util.h"

namespace installer {

LanguageList GetLanguageList() {
  LanguageList result;

  const QString content(ReadFile(RESOURCES_DIR"/languages.json"));
  const QJsonArray lang_list =
      QJsonDocument::fromJson(content.toUtf8()).array();
  for(const QJsonValue& lang_value : lang_list) {
    const QJsonObject obj = lang_value.toObject();
    LanguageItem item;
    item.name = obj.value("name").toString();
    item.locale = obj.value("locale").toString();
    item.lc_all = obj.value("lc_all").toString();
    item.local_name = obj.value("local_name").toString();
    result.append(item);
  }

  return result;
}

}  // namespace installer