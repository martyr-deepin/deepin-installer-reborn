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

#ifndef INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H

#include <QObject>
#include <partman/operation.h>

#include "partman/device.h"
#include "ui/delegates/advanced_validate_state.h"

namespace installer {

// Partition delegate used in AdvancedPartitionFrame and other sub frame pages.
class AdvancedPartitionDelegate : public QObject {
  Q_OBJECT
 public:
  explicit AdvancedPartitionDelegate(QObject* parent = nullptr);

  const DeviceList& real_devices() const { return real_devices_; }
  const DeviceList& virtual_devices() const { return virtual_devices_; }

  // Get alternative partition type. Used while creating a new partition.
  // |partition| is an unallocated partition.
  bool canAddLogical(const Partition::Ptr partition) const;
  bool canAddPrimary(const Partition::Ptr partition) const;

  // Get all supported filesystems.
  FsTypeList getFsTypeList() const;

  // Get all supported filesystems for /boot folder.
  FsTypeList getBootFsTypeList() const;

  // Get all available mount points, defined in settings file.
  QStringList getMountPoints() const;

  // Get human readable operation descriptions.
  QStringList getOptDescriptions() const;

  // Get real partition on disk where |virtual_partition| is located.
  Partition::Ptr getRealPartition(const Partition::Ptr virtual_partition) const;

  QList<Device::Ptr> getAllUsedDevice() const;

  // Returns true if current boot mode is mbr or any system is found on disks
  // with msdos partition table.
  bool isMBRPreferred() const;

  // Check whether device at |device_path| is appropriate for current system.
  bool isPartitionTableMatch(const QString& device_path) const;

  const OperationList& operations() const { return operations_; };

  // Set boot flag of root partition. Call this before operations() and after
  // validate().
  // Returns false if no appropriate partition can be set as bootable.
  bool setBootFlag();

  // Check whether partition operations are appropriate.
  //  * / partition is set and large enough;
  //  * An EFI partition exists if EFI mode is on;
  AdvancedValidateStates validate() const;

 signals:
  // Emitted when virtual device list is updated.
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
  void deletePartition(const Partition::Ptr partition);
  void formatPartition(const Partition::Ptr partition,
                       FsType fs_type,
                       const QString& mount_point);

  // Save real device list when it is refreshed.
  void onDeviceRefreshed(const DeviceList& devices);

  // Write partitioning settings to file.
  void onManualPartDone(const DeviceList& devices);

  // Refresh virtual device list based on current operations.
  void refreshVisual();

  // Clear mount point of operation.new_partition with value |mount_point|.
  void resetOperationMountPoint(const QString& mount_point);

  // Set bootloader path to |path|.
  void setBootloaderPath(const QString& path);

  bool unFormatPartition(const Partition::Ptr partition);

  void updateMountPoint(const Partition::Ptr partition, const QString& mount_point);

 private:
  DeviceList real_devices_;
  DeviceList virtual_devices_;

  QString bootloader_path_;

  // Currently defined operations.
  OperationList operations_;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H
