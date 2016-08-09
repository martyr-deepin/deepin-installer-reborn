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

// PartitionManager proxy layer.
class PartitionDelegate : public QObject {
  Q_OBJECT

 public:
  explicit PartitionDelegate(QObject* parent = nullptr);

  void autoConf();

  service::DeviceList devices;

 signals:
  void deviceRefreshed();

 private:
  void initConnections();

  service::PartitionManager* partition_manager_ = nullptr;
  QThread* partition_thread_ = nullptr;

 private slots:
  void onDevicesRefreshed(const service::DeviceList& devices);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_PARTITION_DELEGATE_H
