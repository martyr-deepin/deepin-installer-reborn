// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_PARTITION_DELEGATE_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_PARTITION_DELEGATE_H

#include <QObject>
class QThread;

#include "service/partition_manager_structs.h"
namespace service {
class PartitionManager;
}  // namespace service

namespace ui {

// 100M, filter partition size.
const qint64 kMinimumPartitionSizeToDisplay = 100 * 1024 * 1024;

enum class PartitionOperations {
  Null,
  Create,
  Delete,
  Format,
};

struct PartitionWrap {
  service::Partition partition;
  PartitionOperations ops = PartitionOperations::Null;
  QString mount_point;
  service::FsType new_fs_type = service::FsType::Empty;
};

typedef QList<PartitionWrap> PartitionWrapList;

struct DeviceWrap {
  service::Device device;
  PartitionWrapList partitions;
};

typedef QList<DeviceWrap> DeviceWrapList;

// PartitionManager proxy layer.
class PartitionDelegate : public QObject {
  Q_OBJECT

 public:
  explicit PartitionDelegate(QObject* parent = nullptr);
  ~PartitionDelegate();

  void autoConf();

  DeviceWrapList devices;

 signals:
  // Emitted after scanning local disk devices.
  void deviceRefreshed();

  // Emitted when a specific partition is created/edited/deleted.
  void partitionEdited();

  // Emitted when system root partition is updated.
  void rootPartitionUpdated();

 private:
  void initConnections();

  service::PartitionManager* partition_manager_ = nullptr;
  QThread* partition_thread_ = nullptr;

 private slots:
  void onDevicesRefreshed(const service::DeviceList& devices);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_PARTITION_DELEGATE_H
