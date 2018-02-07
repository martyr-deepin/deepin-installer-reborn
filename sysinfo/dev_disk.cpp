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

#include "sysinfo/dev_disk.h"

#include <QDir>

namespace installer {

namespace {

QHash<QString, QString> ParseDevDir(const QString& dir_name) {
  QHash<QString, QString> result;

  QDir dir(dir_name);
  if (!dir.exists()) {
    // Returns an empty list if that folder does not exist.
    return result;
  }

  const QFileInfoList items = dir.entryInfoList(QDir::NoDotAndDotDot |
                                                QDir::Files);
  for (const QFileInfo& info : items) {
    result.insert(info.canonicalFilePath(), info.fileName());
  }

  return result;
};

}  // namespace

QHash<QString, QString> ParseLabelDir() {
  return ParseDevDir("/dev/disk/by-label");
}

QHash<QString, QString> ParsePartLabelDir() {
  return ParseDevDir("/dev/disk/by-partlabel");
};

QHash<QString, QString> ParseUUIDDir() {
  return ParseDevDir("/dev/disk/by-uuid");
}

}  // namespace installer