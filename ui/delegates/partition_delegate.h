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

// PartitionManager proxy layer. It is shared among partition sub frames.
// PartitionManager is invisible in frame pages.
class PartitionDelegate : public QObject {
  Q_OBJECT

 public:
  explicit PartitionDelegate(QObject* parent = nullptr);
  ~PartitionDelegate();

  // Notify PartitionManager to do auto-part
  void autoConf();

  // Get alternative partition type. Used while creating a new partition.
  // |partition| is an unallocated partition.
  bool canAddPrimary(const Partition& partition) const;
  bool canAddLogical(const Partition& partition) const;

  // Get a list of devices found by libparted.
  const DeviceList& devices() const { return devices_; }

  // Get human readable operation descriptions.
  QStringList getOperationDescriptions() const;

  // Notifies partition manager to scan devices.
  void scanDevices() const;

  // Get all available mount points, defined in settings file.
  const QStringList& allMountPoints();

  // Get all supported fs type.
  const FsTypeList& getFsTypes();

 signals:
  // Emitted after scanning local disk devices or partition is edited.
  void deviceRefreshed();

  // Emitted when partition job is done.
  void autoPartDone(bool ok);
  void manualPartDone(bool ok);

 public slots:
  // Notify partition_manager to do manual partition operations.
  // NOTE(xushaohua): This action can not be undo.
  void doManualPart();

  // Set bootloader path to |bootloader_path|
  // This slot is used only in SelectBootloaderFrame.
  void setBootloaderPath(const QString bootloader_path);

  // Operation helpers.
  // Create a new partition.
  void createPartition(const Partition& partition,
                       PartitionType partition_type,
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

  // Refresh virtual device list based on physical device list and
  // operator list. It is used when a new operator is created.
  void refreshVisual();

 private:
  void initConnections();

  void createPrimaryPartition(const Partition& partition,
                              bool align_start,
                              FsType fs_type,
                              const QString& mount_point,
                              qint64 total_sectors);
  void createLogicalPartition(const Partition& partition,
                              bool align_start,
                              FsType fs_type,
                              const QString& mount_point,
                              qint64 total_sectors);
  void removeEmptyExtendedPartition(const PartitionList& partitions);

  PartitionManager* partition_manager_ = nullptr;
  QThread* partition_thread_ = nullptr;

  // device list which are managed by this delegate.
  DeviceList devices_;
  // Physical device list.
  DeviceList real_devices_;

  OperationList operations_;

  QStringList all_mount_points_;
  FsTypeList fs_types_;
  QString bootloader_path_;

 private slots:
  // Update physical and virtual partition list.
  void onDevicesRefreshed(const DeviceList& devices);

  void onManualPartDone(bool ok);
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_PARTITION_DELEGATE_H
