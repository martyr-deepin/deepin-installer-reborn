// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_PROC_PARTITIONS_H
#define INSTALLER_SYSINFO_PROC_PARTITIONS_H

#include <QList>
#include <QString>

namespace installer {

// Parse /proc/partitions file.
struct PartitionItem {
  int major;
  int minor;
  qint64 blocks;  // Size in kib.
  QString name;
};

typedef QList<PartitionItem> PartitionItemList;

PartitionItemList ParsePartitionItems();

}  // namespace installer

#endif  // INSTALLER_SYSINFO_PROC_PARTITIONS_H
