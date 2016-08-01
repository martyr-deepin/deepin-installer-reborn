// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SYSINFO_PROC_PARTITIONS_H
#define DEEPIN_INSTALLER_REBORN_SYSINFO_PROC_PARTITIONS_H

#include <QList>
#include <QString>

namespace sysinfo {

// Parse /proc/partitions file.
struct PartitionItem {
  int major;
  int minor;
  qint64 blocks;
  QString name;
};

typedef QList<PartitionItem> PartitionItemList;

PartitionItemList ParsePartitionItems();

}  // namespace sysinfo

#endif  // DEEPIN_INSTALLER_REBORN_SYSINFO_PROC_PARTITIONS_H
