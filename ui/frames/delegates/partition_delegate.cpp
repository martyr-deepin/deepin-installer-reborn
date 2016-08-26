// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/delegates/partition_delegate.h"

#include <QThread>

#include "service/partition_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "service/signal_manager.h"

namespace ui {

PartitionDelegate::PartitionDelegate(QObject* parent)
    : QObject(parent),
      devices(),
      partition_manager_(new service::PartitionManager()),
      partition_thread_(new QThread()) {
  this->setObjectName(QStringLiteral("partition_delegate"));

  partition_manager_->moveToThread(partition_thread_);
  partition_thread_->start();

  this->initConnections();

  // If auto-part is not set, scan devices right now.
  if (!service::GetSettingsBool(service::kPartitionDoAutoPart)) {
    emit partition_manager_->refreshDevices();
  }
}

PartitionDelegate::~PartitionDelegate() {
  delete partition_manager_;
  partition_manager_ = nullptr;

  // Quit background thread explicitly.
  partition_thread_->quit();
  partition_thread_->wait();
  delete partition_thread_;
  partition_thread_ = nullptr;
}

void PartitionDelegate::autoConf() {
  emit partition_manager_->autoPart();
}

void PartitionDelegate::initConnections() {
  service::SignalManager* signal_manager = service::SignalManager::instance();
  connect(partition_manager_, &service::PartitionManager::autoPartDone,
          signal_manager, &service::SignalManager::autoPartDone);
  connect(partition_manager_, &service::PartitionManager::manualPartDone,
          signal_manager, &service::SignalManager::manualPartDone);

  connect(partition_manager_, &service::PartitionManager::devicesRefreshed,
          this, &PartitionDelegate::onDevicesRefreshed);
}

void PartitionDelegate::onDevicesRefreshed(const service::DeviceList& devices) {
  this->devices.clear();
  for (const service::Device& device : devices) {
    DeviceWrap device_wrap;
    device_wrap.device = device;
    PartitionWrapList partitions;
    for (const service::Partition& partition : device.partitions) {
      PartitionWrap partition_wrap;
      partition_wrap.partition = partition;
      partitions.append(partition_wrap);
    }
    device_wrap.partitions = partitions;

    this->devices.append(device_wrap);
  }

  emit this->deviceRefreshed();
}

}  // namespace ui
