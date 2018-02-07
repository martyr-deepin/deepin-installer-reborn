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

#include "partman/utils.h"

#include <parted/parted.h>
#include <string.h>
#include <QDebug>
#include <QDir>

#include "partman/structs.h"
#include "sysinfo/proc_partitions.h"

namespace installer {

qint64 GetMaximumDeviceSize() {
  const PartitionItemList list = ParsePartitionItems();
  qint64 result = 0;
  for (const PartitionItem item : list) {
    result = qMax(result, item.blocks);
  }

  // Convert kib to bytes.
  return result * kKibiByte;
}

PartitionTableType GetPrimaryDiskPartitionTable() {
  // TODO(xushaohua): Read path to the first device.
  PedDevice* device = ped_device_get("/dev/sda");
  if (device == NULL) {
    return PartitionTableType::Unknown;
  }
  PedDiskType* disk_type = ped_disk_probe(device);
  if (disk_type == NULL) {
    return PartitionTableType::Empty;
  }

  const QString disk_type_name(disk_type->name);
  if (disk_type_name == kPartitionTableMsDos) {
    return PartitionTableType::MsDos;
  } else if (disk_type_name == kPartitionTableGPT) {
    return PartitionTableType::GPT;
  }

  return PartitionTableType::Others;
}

}  // namespace installer