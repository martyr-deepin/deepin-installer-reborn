// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_DELEGATES_PARTITION_DELEGATE_H
#define DEEPIN_INSTALLER_REBORN_UI_DELEGATES_PARTITION_DELEGATE_H

#include <QList>
#include <QObject>
class QThread;

#include "partman/device.h"
#include "partman/operation.h"

namespace partman {
class PartitionManager;
}  // namespace partman

namespace ui {

// If partition size is less than this value, hide it from partition list.
const qint64 kMinimumPartitionSizeToDisplay = 10 * kMebiByte;

enum class PartitionType {
  PrimaryOnly,
  LogicalOnly,
  PrimaryOrLogical,
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

  const partman::DeviceList& devices() const { return devices_; }

  // Get alternative partition type.
  PartitionType getPartitionType(const partman::Partition& partition) const;

  // Get mount point based on fs type.
  const QStringList& getMountPoints();
  // Remove |mount_point| from result of getMountPoints().
  void useMountPoint(const QString& mount_point);

  // Get all supported fs type.
  const partman::FsTypeList& getFsTypes();

  // Operation helpers.
  // Create a new partition.
  void createPartition(const partman::Partition& partition,
                       bool is_primary,
                       bool align_start,
                       partman::FsType fs_type,
                       const QString& mount_point,
                       qint64 total_sectors);
  // Delete a |partition|.
  void deletePartition(const partman::Partition& partition);
  // Format a partition |partition|.
  void formatPartition(const partman::Partition& partition,
                       partman::FsType fs_type,
                       const QString& mount_point);
  // Change mount point of |partition|.
  void updateMountPoint(const partman::Partition& partition,
                        const QString& mount_point);

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

  partman::PartitionManager* partition_manager_ = nullptr;
  QThread* partition_thread_ = nullptr;

  // device list which are managed by this delegate.
  partman::DeviceList devices_;
  // Physical device list.
  partman::DeviceList real_devices_;

  partman::OperationList operations_;

  QStringList all_mount_points_;
  QStringList unused_mount_points_;
  partman::FsTypeList fs_types_;

 private slots:
  void onDevicesRefreshed(const partman::DeviceList& devices);
  void onManualPartDone(bool ok,
                        const QList<QPair<QString, QString>>& mount_point_pair);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_DELEGATES_PARTITION_DELEGATE_H
