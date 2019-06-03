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

#ifndef INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H

#include <QObject>
#include <partman/operation.h>

#include "partman/device.h"

namespace installer {

enum class SimpleValidateState {
  Ok,
  MaxPrimPartErr,  // All primary partition numbers are used.
  RootMissing,
  RootTooSmall,
};

// Partition delegate used in SimplePartitionFrame.
class SimplePartitionDelegate : public QObject {
  Q_OBJECT
 public:
  // Note that this objects of this class does not take ownership of model.
  // Nor does it handle life time of model.
  explicit SimplePartitionDelegate(QObject* parent = nullptr);

  // Get physical device list.
  const DeviceList& real_devices() const { return real_devices_; }

  // Get virtual device list.
  const DeviceList& virtual_devices() const { return virtual_devices_; }

  // Get alternative partition type. Used while creating a new partition.
  // |partition| is an unallocated partition.
  bool canAddLogical(const Partition::Ptr partition) const;
  bool canAddPrimary(const Partition::Ptr partition) const;

  // Get human readable operation descriptions.
  QStringList getOptDescriptions() const;

  // Returns true if current boot mode is mbr or any system is found on disks
  // with msdos partition table.
  bool isMBRPreferred() const;

  // Check whether device at |device_path| is appropriate for current system.
  bool isPartitionTableMatch(const QString& device_path) const;

  // Get current operation list.
  const OperationList& operations() const { return operations_; }

  // Select |partition| as root partition.
  // It may be separated into two partitions if needed.
  // Call validate() to check whether this |partition| is appropriate before
  // using it.
  void selectPartition(const Partition::Ptr partition);

  inline Partition::Ptr selectedPartition() const { return selected_partition_; }

  // Set boot flag of root partition. Call this before operations() and after
  // validate().
  // Returns false if no appropriate partition can be set as bootable.
  bool setBootFlag();

  // Validate whether selected partition is appropriate.
  SimpleValidateState validate() const;

 signals:
  void deviceRefreshed(const DeviceList& devices);

 public slots:
  bool createPartition(const Partition::Ptr partition,
                       PartitionType partition_type,
                       bool align_start,
                       FsType fs_type,
                       const QString& mount_point,
                       qint64 total_sectors);
  bool createLogicalPartition(const Partition::Ptr partition,
                              bool align_start,
                              FsType fs_type,
                              const QString& mount_point,
                              qint64 total_sectors);
  bool createPrimaryPartition(const Partition::Ptr partition,
                              PartitionType partition_type,
                              bool align_start,
                              FsType fs_type,
                              const QString& mount_point,
                              qint64 total_sectors);
  Partition::Ptr deletePartition(const Partition::Ptr partition);
  void formatPartition(const Partition::Ptr partition,
                       FsType fs_type,
                       const QString& mount_point);

  // Create and append operations to whole device at |device_path|:
  //  * A /boot partition;
  //  * A linux-swap partition;
  //  * A root partition;
  bool formatWholeDevice(const QString& device_path, PartitionTableType type);

  // Save real device list when it is refreshed.
  void onDeviceRefreshed(const DeviceList& devices);

  // Write partitioning settings to file.
  void onManualPartDone(const DeviceList& devices);

  // Clear operation list.
  void resetOperations();

  // Update bootloader settings to |path|.
  void setBootloaderPath(const QString& path);

 private:
  DeviceList real_devices_;
  DeviceList virtual_devices_;
  QString bootloader_path_;
  OperationList operations_;
  Partition::Ptr selected_partition_;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H
