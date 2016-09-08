// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_PARTITION_DELEGATE_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_PARTITION_DELEGATE_H

#include <QObject>
class QThread;

#include "partman/device.h"
#include "ui/frames/delegates/operation.h"
namespace partman {
class PartitionManager;
}  // namespace partman

namespace ui {

// 100M, filter partition size.
const qint64 kMinimumPartitionSizeToDisplay = 100 * 1024 * 1024;

// PartitionManager proxy layer.
class PartitionDelegate : public QObject {
  Q_OBJECT

 public:
  explicit PartitionDelegate(QObject* parent = nullptr);
  ~PartitionDelegate();

  // Notify PartitionManager to do auto-part
  void autoConf();

  const partman::DeviceList& devices() const { return devices_; }

  // Delete a partition at |partition_path|.
  void deletePartition(const QString& partition_path);

  // Get mount point based on fs type.
  const QStringList& getMountPoints();

  // Get all supported fs type.
  const partman::FsTypeList& getFsTypes();

 signals:
  // Emitted after scanning local disk devices.
  void deviceRefreshed();

  // Emitted when a specific partition is created/edited/deleted.
  void partitionEdited();

 private:
  void initConnections();

  partman::PartitionManager* partition_manager_ = nullptr;
  QThread* partition_thread_ = nullptr;

  partman::DeviceList devices_;
  OperationList operations_;

  QStringList all_mount_points_;
  QStringList unused_mount_points_;
  partman::FsTypeList fs_types_;

 private slots:
  void onDevicesRefreshed(const partman::DeviceList& devices);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_PARTITION_DELEGATE_H
