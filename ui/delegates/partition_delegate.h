// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_PARTITION_DELEGATE_H
#define INSTALLER_UI_DELEGATES_PARTITION_DELEGATE_H

#include <QList>
#include <QObject>
class QThread;

#include "partman/device.h"
#include "partman/operation.h"

namespace installer {

class PartitionManager;

// If partition size is less than this value, hide it from partition list.
const qint64 kMinimumPartitionSizeToDisplay = 10 * kMebiByte;

enum class SupportedPartitionType {
  PrimaryOnly,
  LogicalOnly,
  PrimaryOrLogical,
  Invalid,
};

// PartitionManager proxy layer. It is shared among partition sub frames.
// PartitionManager is invisible in frame pages.
class PartitionDelegate : public QObject {
  Q_OBJECT

 public:
  explicit PartitionDelegate(QObject* parent = nullptr);
  ~PartitionDelegate();

  // Notify PartitionManager to do auto-part
  void autoConf();

  const DeviceList& devices() const { return devices_; }

  // Notifies partition manager to scan devices.
  void scanDevices() const;

  // Get alternative partition type.
  SupportedPartitionType getPartitionType(const Partition& partition) const;

  // Get mount point based on fs type.
  const QStringList& getMountPoints();
  // Remove |mount_point| from result of getMountPoints().
  void useMountPoint(const QString& mount_point);

  // Get all supported fs type.
  const FsTypeList& getFsTypes();

  // Operation helpers.
  // Create a new partition.
  void createPartition(const Partition& partition,
                       bool is_primary,
                       bool align_start,
                       FsType fs_type,
                       const QString& mount_point,
                       qint64 total_sectors);
  // Delete a |partition|.
  void deletePartition(const Partition& partition);
  // Format a partition |partition|.
  void formatPartition(const Partition& partition,
                       FsType fs_type,
                       const QString& mount_point);
  // Change mount point of |partition|.
  void updateMountPoint(const Partition& partition, const QString& mount_point);

 signals:
  // Emitted after scanning local disk devices.
  void deviceRefreshed();

  // Emitted when a specific partition is created/edited/deleted.
  void partitionEdited();

 public slots:
  void doManualPart();

 private:
  void initConnections();
  void refreshVisual();

  PartitionManager* partition_manager_ = nullptr;
  QThread* partition_thread_ = nullptr;

  // device list which are managed by this delegate.
  DeviceList devices_;
  // Physical device list.
  DeviceList real_devices_;

  OperationList operations_;

  QStringList all_mount_points_;
  QStringList unused_mount_points_;
  FsTypeList fs_types_;

 private slots:
  void onDevicesRefreshed(const DeviceList& devices);
  void onManualPartDone(bool ok,
                        const QList<QPair<QString, QString>>& mount_point_pair);
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_PARTITION_DELEGATE_H
