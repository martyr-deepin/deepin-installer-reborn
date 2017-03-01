// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_DELEGATE_H

#include <QObject>
#include <partman/operation.h>

#include "partman/device.h"

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
  bool canAddPrimary(const Partition& partition) const;
  bool canAddLogical(const Partition& partition) const;

  // Get all supported fs type.
  FsTypeList getFsTypeList() const;

  // Get all available mount points, defined in settings file.
  QStringList getMountPoints() const;

  // Get real partition on disk where |virtual_partition| is located.
  Partition getRealPartition(const Partition& virtual_partition) const;

  // Check whether device at |device_path| is appropriate for current system.
  bool isPartitionTableMatch(const QString& device_path) const;

  const OperationList& operations() const { return operations_; };

 signals:
  void deviceRefreshed(const DeviceList& devices);

 public slots:
  void createPartition(const Partition& partition,
                       PartitionType partition_type,
                       bool align_start,
                       FsType fs_type,
                       const QString& mount_point,
                       qint64 total_sectors);

  void formatPartition(const Partition& partition,
                       FsType fs_type,
                       const QString& mount_point);

  void refreshVisual();

  // Set bootloader path to |path|.
  void setBootloaderPath(const QString& path);

  void unFormatPartition(const Partition& partition);

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
