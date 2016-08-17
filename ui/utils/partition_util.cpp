// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/utils/partition_util.h"

#include <math.h>
#include <QDebug>
#include <QFileInfo>

namespace ui {

QString GetPartitionName(const QString& path) {
  return QFileInfo(path).fileName();
}

QString GetPartitionUsage(qint64 freespace, qint64 total) {
  const qint64 used = total - freespace;
  return QString("%1/%2G").arg(ToGigByte(used)).arg(ToGigByte(total));
}

int ToGigByte(qint64 size) {
  const double m_size = static_cast<double>(size) / 1024.0;
  qDebug() << size << m_size << (m_size / 1024.0) << (m_size / 1024.0 / 1024.0);
  return  static_cast<int>(round(m_size / 1024.0 / 1024.0));
}

}  // namespace ui