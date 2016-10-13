// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/device.h"

namespace installer {

Device::Device()
    : partitions(),
      model(),
      path(),
      length(0),
      heads(0),
      sectors(0),
      cylinders(0),
      cylsize(0),
      sector_size(0),
      max_prims(0),
      read_only(true),
      table(PartitionTableType::Unknown) {
}

QDebug& operator<<(QDebug& debug, const Device& device) {
  debug << "Device: {"
        << "path:" << device.path
        << "model:" << device.model
        << "partition table:" << device.table
        << "max prims:" << device.max_prims
        << "length:" << device.length
        << "sectors:" << device.sectors
        << "sector size:" << device.sector_size
        << "partition list:" << device.partitions
        << "}";
  return debug;
}

int DeviceIndex(const DeviceList& devices, const QString& device_path) {
  for (int i = 0; i < devices.length(); ++i) {
    if (devices.at(i).path == device_path) {
      return i;
    }
  }
  return -1;
}

}  // namespace installer