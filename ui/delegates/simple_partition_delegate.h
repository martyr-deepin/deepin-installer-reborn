// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H

#include <QObject>

#include "partman/device.h"

namespace installer {

class PartitionModel;

// Partition delegate used in SimplePartitionFrame.
class SimplePartitionDelegate : public QObject {
  Q_OBJECT
 public:
  // Note that this objects of this class does not take ownership of model.
  // Nor does it handle life time of model.
  SimplePartitionDelegate(PartitionModel* model, QObject* parent = nullptr);

  const DeviceList& real_devices() const { return real_devices_; }
  const DeviceList& virtual_devices() const { return virtual_devices_; }

  bool canAddLogical(const Partition& partition) const;
  bool canAddPrimary(const Partition& partition) const;

  bool isPartitionTableMatch(const QString& device_path) const;

 signals:
  void realDeviceRefreshed();

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

  void setBootloaderPath(const QString& path);

 private:
  PartitionModel* model_ = nullptr;
  DeviceList real_devices_;
  DeviceList virtual_devices_;
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_SIMPLE_PARTITION_DELEGATE_H
