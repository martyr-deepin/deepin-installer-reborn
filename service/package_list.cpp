// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/package_list.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/file_util.h"

namespace installer {

namespace {

const char kPackageFile[] = RESOURCES_DIR "/select_services_packages.json";

}  // namespace

PackageList GetPackageList() {
  PackageList list;

  const QString content(ReadFile(kPackageFile));
  if (content.isEmpty()) {
    qWarning() << "Failed to read package list file";
    return list;
  }

  const QJsonObject obj = QJsonDocument::fromJson(content.toUtf8()).object();
  list.title = obj.value("title_name").toString();

  const QJsonArray selected = obj.value("selected").toArray();
  for (const QJsonValue& item : selected) {
    list.selected.append(item.toInt());
  }

  const QJsonArray items = obj.value("items").toArray();
  for (const QJsonValue& item : items) {
    list.items.append(item.toString());
  }

  return list;
}

}  // namespace installer