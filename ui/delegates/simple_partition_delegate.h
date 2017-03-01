// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H

#include <QObject>
#include <partman/operation.h>

#include "partman/device.h"

namespace installer {

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
  bool canAddLogical(const Partition& partition) const;
  bool canAddPrimary(const Partition& partition) const;

  // Check whether device at |device_path| is appropriate for current system.
  bool isPartitionTableMatch(const QString& device_path) const;

  // Get current operation list.
  const OperationList& operations() const { return operations_; }

 signals:
  void deviceRefreshed(const DeviceList& devices);

 public slots:
  void resetOperations();

  void createPartition(const Partition& partition,
                       PartitionType part_type,
                       bool align_start,
                       FsType fs_type,
                       const QString& mount_point,
                       qint64 total_sectors);
  Partition deletePartition(const Partition& partition);
  void formatPartition(const Partition& partition,
                       FsType fs_type,
                       const QString& mount_point);

  // Save real device list when it is refreshed.
  void onDeviceRefreshed(const DeviceList& devices);

  // Update bootloader settings to |path|.
  void setBootloaderPath(const QString& path);

 private:
  DeviceList real_devices_;
  DeviceList virtual_devices_;

  QString bootloader_path_;

  OperationList operations_;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H
