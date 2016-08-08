// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/delegates/partition_delegate.h"

#include <QThread>

#include "service/partition_manager.h"
#include "service/signal_manager.h"

namespace ui {

PartitionDelegate::PartitionDelegate(QObject* parent) :
    QObject(parent),
    partition_manager_(new service::PartitionManager()),
    partition_thread_(new QThread(this)) {
  this->setObjectName(QStringLiteral("partition_delegate"));

  partition_manager_->moveToThread(partition_thread_);
  partition_thread_->start();

  this->initConnections();
  emit partition_manager_->refreshDevices();
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
          this, &PartitionDelegate::deviceRefreshed);
  connect(partition_manager_, &service::PartitionManager::devicesRefreshed,
          this, &PartitionDelegate::onDevicesRefreshed);
}

void PartitionDelegate::onDevicesRefreshed() {

}

}  // namespace ui