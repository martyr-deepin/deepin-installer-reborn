// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_DEVICE_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_DEVICE_H

#include <QList>

#include "partman/partition.h"

namespace partman {

class Device {
 public:
  Device();

  PartitionList partitions;
  QString model;
  QString path;
  qint64 length;  // device length
  qint64 heads;
  qint64 sectors;
  qint64 cylinders;
  qint64 cylsize;
  QString disk_type;
  qint64 sector_size;  // logical sector size
  int max_prims;
  bool read_only;

  PartitionTableType table;
};

typedef QList<Device> DeviceList;

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_DEVICE_H
