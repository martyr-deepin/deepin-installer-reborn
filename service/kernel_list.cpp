// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/kernel_list.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/file_util.h"

namespace installer {

namespace {

const char kKernelFile[] = RESOURCES_DIR "/select_machine_kernel.json";

}  // namespace

KernelList GetKernelList() {
  KernelList list;

  const QString content(ReadFile(kKernelFile));
  if (content.isEmpty()) {
    qWarning() << "Failed to read kernel list file";
    return list;
  }

  const QJsonObject obj = QJsonDocument::fromJson(content.toUtf8()).object();
  list.title = obj.value("title_name").toString();

  // Default is -1.
  list.selected = obj.value("selected").toInt(-1);

  const QJsonArray items = obj.value("items").toArray();
  for (const QJsonValue& item : items) {
    list.items.append(item.toString());
  }

  return list;
}

}  // namespace installer