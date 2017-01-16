// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_PARTMAN_DEVICE_H
#define INSTALLER_PARTMAN_DEVICE_H

#include <QDebug>
#include <QList>

#include "partman/partition.h"

namespace installer {

class Device {
 public:
  Device();

  PartitionList partitions;
  QString model;
  QString path;
  qint64 length;  // device length, in sector, total sectors.
  qint64 heads;
  qint64 sectors;
  qint64 cylinders;
  qint64 cylsize;
  qint64 sector_size;  // logical sector size
  int max_prims;
  bool read_only;

  PartitionTableType table;

  // Returns size of device. Returns -1 if failed.
  qint64 getByteLength() const;
};
QDebug& operator<<(QDebug& debug, const Device& device);

typedef QList<Device> DeviceList;

// Get index of device object with |device_path|. Returns -1 if not found.
int DeviceIndex(const DeviceList& devices, const QString& device_path);

}  // namespace installer

#endif  // INSTALLER_PARTMAN_DEVICE_H
