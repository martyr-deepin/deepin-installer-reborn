// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/inner/partition_label.h"

#include <QProcess>
#include <QString>

#include "base/string_util.h"

namespace service {

QString ReadBtrfsLabel(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadEFILabel(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadExt2Label(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadFat16Label(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadHfsLabel(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadJfsLabel(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadNtfsLabel(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadReiser4Label(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadReiserfsLabel(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

QString ReadXfsLabel(const QString& path) {
  Q_UNUSED(path);
  return QString();
}

}  // namespace service