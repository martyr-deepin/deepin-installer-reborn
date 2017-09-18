/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

qint64 Device::getByteLength() const {
  if (length > 0) {
    return length * sector_size;
  } else {
    return -1;
  }
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