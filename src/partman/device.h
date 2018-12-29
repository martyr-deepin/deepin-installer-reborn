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

#ifndef INSTALLER_PARTMAN_DEVICE_H
#define INSTALLER_PARTMAN_DEVICE_H

#include <QDebug>
#include <QList>
#include <QSharedPointer>

#include "partman/partition.h"

namespace installer {

class Device {
 public:
  Device();
  Device(const Device &device);

  typedef QSharedPointer<Device> Ptr;

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

  bool operator==(const Device &device);
};
QDebug& operator<<(QDebug& debug, const Device& device);
QDebug& operator<<(QDebug& debug, const Device::Ptr device);

typedef QList<Device::Ptr> DeviceList;

// Get index of device object with |device_path|. Returns -1 if not found.
int DeviceIndex(const DeviceList& devices, const QString& device_path);

}  // namespace installer

#endif  // INSTALLER_PARTMAN_DEVICE_H
