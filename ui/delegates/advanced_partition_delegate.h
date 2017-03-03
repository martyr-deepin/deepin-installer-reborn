// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H

#include <QObject>
#include <partman/operation.h>

#include "partman/device.h"

namespace installer {

enum class ValidateState {
  BootFsInvalid,  // Filesystem used for /boot is not in supported fs list.
  BootTooSmall,
  EfiMissing,
  EfiTooSmall,
  RootMissing,
  RootTooSmall,
};
typedef QList<ValidateState> ValidateStates;

// Partition delegate used in AdvancedPartitionFrame and other sub frame pages.
class AdvancedPartitionDelegate : public QObject {
  Q_OBJECT
 public:
  explicit AdvancedPartitionDelegate(QObject* parent = nullptr);

  const DeviceList& real_devices() const { return real_devices_; }
  const DeviceList& virtual_devices() const { return virtual_devices_; }

  // Get alternative partition type. Used while creating a new partition.
  // |partition| is an unallocated partition.
  bool canAddLogical(const Partition& partition) const;
  bool canAddPrimary(const Partition& partition) const;

  // Get all supported filesystems.
  FsTypeList getFsTypeList() const;

  // Get all supported filesystems for /boot folder.
  FsTypeList getBootFsTypeList() const;

  // Get all available mount points, defined in settings file.
  QStringList getMountPoints() const;

  // Get human readable operation descriptions.
  QStringList getOptDescriptions() const;

  // Get real partition on disk where |virtual_partition| is located.
  Partition getRealPartition(const Partition& virtual_partition) const;

  // Check whether device at |device_path| is appropriate for current system.
  bool isPartitionTableMatch(const QString& device_path) const;

  const OperationList& operations() const { return operations_; };

  // Check whether partition operations are appropriate.
  //  * / partition is set and large enough;
  //  * An EFI partition exists if EFI mode is on;
  ValidateStates validate() const;

 signals:
  // Emitted when virtual device list is updated.
  void deviceRefreshed(const DeviceList& devices);

 public slots:
  bool createPartition(const Partition& partition,
                       PartitionType partition_type,
                       bool align_start,
                       FsType fs_type,
                       const QString& mount_point,
                       qint64 total_sectors);
  bool createLogicalPartition(const Partition& partition,
                              bool align_start,
                              FsType fs_type,
                              const QString& mount_point,
                              qint64 total_sectors);
  bool createPrimaryPartition(const Partition& partition,
                              PartitionType partition_type,
                              bool align_start,
                              FsType fs_type,
                              const QString& mount_point,
                              qint64 total_sectors);
  void deletePartition(const Partition& partition);
  void formatPartition(const Partition& partition,
                       FsType fs_type,
                       const QString& mount_point);

  // Save real device list when it is refreshed.
  void onDeviceRefreshed(const DeviceList& devices);

  // Write partitioning settings to file.
  void onManualPartDone();

  // Refresh virtual device list based on current operations.
  void refreshVisual();

  // Clear mount point of operation.new_partition with value |mount_point|.
  void resetOperationMountPoint(const QString& mount_point);

  // Set bootloader path to |path|.
  void setBootloaderPath(const QString& path);

  bool unFormatPartition(const Partition& partition);

  void updateMountPoint(const Partition& partition, const QString& mount_point);

 private:
  DeviceList real_devices_;
  DeviceList virtual_devices_;

  QString bootloader_path_;

  // Currently defined operations.
  OperationList operations_;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H
