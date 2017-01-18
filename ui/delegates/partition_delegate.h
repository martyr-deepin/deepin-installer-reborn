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
// Operations generated in SimplePartitionFrame and AdvancedPartitionFrame
// are stored in different lists, so that operations can be reverted or cleared
// in SimplePartitionFrame.
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

  // Get a list of devices found by libparted, and modified by operations.
  const DeviceList& devices() const { return devices_; }

  // Get a list of devices found by libparted, .
  const DeviceList& realDevices() const { return real_devices_; }

  // Get human readable operation descriptions.
  QStringList getOperationDescriptions() const;

  // Get real partition on disk in which |virtual_partition| is located.
  Partition getRealPartition(const Partition& virtual_partition) const;

  // Notifies partition manager to scan devices.
  void scanDevices() const;

  // Get all available mount points, defined in settings file.
  const QStringList& allMountPoints();

  // Get all supported fs type.
  const FsTypeList& getFsTypes();

 signals:
  // Emitted after scanning local disk devices or partition is edited.
  void deviceRefreshed();

  // Emitted after scanning local disk devices.
  void realDeviceRefreshed();

  // Emitted when partition job is done.
  void autoPartDone(bool ok);
  void manualPartDone(bool ok);

 public slots:
  // Notify partition_manager to do manual partition operations.
  // Validate operations before calling this method.
  // If |simple_mode| is true, use simple_operations_.
  // Else use operations_.
  // NOTE(xushaohua): This action can not be undo.
  void doManualPart(bool simple_mode);

  void createSimplePartition(const Partition& partition,
                             PartitionType partition_type,
                             bool align_start,
                             FsType fs_type,
                             const QString& mount_point,
                             qint64 total_sectors);

  // Delete |partition| and returns newly created unallocated partition.
  Partition deleteSimplePartition(const Partition& partition);

  // Format a primary partition |partition|.
  // Used in SimplePartitionFrame.
  void formatSimplePartition(const Partition& partition,
                             FsType fs_type,
                             const QString& mount_point);
  // Clear simple_operations_ list.
  void resetSimpleOperations();

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
  // Revert format operation on |partition|.
  // This method does make sense only if partition.status is Format.
  // Returns true if that operation is found and removed.
  bool unFormatPartition(const Partition& partition);
  // Change mount point of |partition|.
  void updateMountPoint(const Partition& partition, const QString& mount_point);

  // Refresh virtual device list based on physical device list and
  // operator list. It is used when a new operator is created.
  void refreshVisual();

  // Set bootloader path to |bootloader_path|
  // This slot is used only in SelectBootloaderFrame.
  void setBootloaderPath(const QString bootloader_path);

 private:
  void initConnections();

  // Create primary partition. If |is_simple| is true, append operation
  // to simple_operations_ list.
  bool createPrimaryPartition(const Partition& partition,
                              PartitionType partition_type,
                              bool align_start,
                              FsType fs_type,
                              const QString& mount_point,
                              qint64 total_sectors,
                              bool is_simple);

  // Create logical partition. If |is_simple| is true, append operation to
  // simple_partitions_ list.
  bool createLogicalPartition(const Partition& partition,
                              bool align_start,
                              FsType fs_type,
                              const QString& mount_point,
                              qint64 total_sectors,
                              bool is_simple);

  // Clear mount point of operation.new_partition with value |mount_point|.
  void resetOperationMountPoint(const QString& mount_point);

  PartitionManager* partition_manager_ = nullptr;
  QThread* partition_thread_ = nullptr;

  // device list which are managed by this delegate.
  DeviceList devices_;
  // Physical device list.
  DeviceList real_devices_;

  // simple_operations_ is only used for SimplePartitionFrame
  OperationList simple_operations_;
  OperationList operations_;

  QStringList all_mount_points_;
  FsTypeList fs_types_;
  QString bootloader_path_;

  // Use simple mode or advanced mode to do manual part.
  bool simple_mode_;

 private slots:
  // Update physical and virtual partition list.
  void onDevicesRefreshed(const DeviceList& devices);

  void onManualPartDone(bool ok);
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_PARTITION_DELEGATE_H
