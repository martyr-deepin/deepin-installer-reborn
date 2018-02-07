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

#ifndef INSTALLER_PARTMAN_PARTITION_MANAGER_H
#define INSTALLER_PARTMAN_PARTITION_MANAGER_H

#include <QList>
#include <QObject>

#include "partman/device.h"
#include "partman/operation.h"

namespace installer {

class PartitionManager : public QObject {
  Q_OBJECT

 public:
  explicit PartitionManager(QObject* parent = nullptr);
  ~PartitionManager();

 signals:
  // Notify PartitionManager to scan devices.
  // If |umount| is true, umount partitions before scanning.
  // If |enable_os_prober| is true, detect os types in partitions.
  void refreshDevices(bool umount, bool enable_os_prober);
  void devicesRefreshed(const DeviceList& devices);

  // Create new partition |table| at |device_path|.
  void createPartitionTable(const QString& device_path,
                            PartitionTableType table);

  // Run auto part script at |script_path|.
  void autoPart(const QString& script_path);
  // Emitted after auto_part.sh script is executed and exited.
  // |ok| is true if that script exited 0.
  void autoPartDone(bool ok);

  void manualPart(const OperationList& operations);

  // Emitted when manualPart() is done.
  // |ok| is true when all operations in operation list are done successfully,
  // and |devices| contains real device list with mount-point.
  void manualPartDone(bool ok, const DeviceList& devices);

 private:
  void initConnections();

  bool enable_os_prober_;

 private slots:
  void doCreatePartitionTable(const QString& device_path,
                              PartitionTableType table);

  void doRefreshDevices(bool umount, bool enable_os_prober);
  void doAutoPart(const QString& script_path);
  void doManualPart(const OperationList& operations);
};

// Scan all disk devices on this machine.
// Detect OS types if |enable_os_prober| is true.
// Do not call this function directly, use PartitionManager instead.
DeviceList ScanDevices(bool enable_os_prober);

}  // namespace installer

#endif  // INSTALLER_PARTMAN_PARTITION_MANAGER_H
