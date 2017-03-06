// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/partition_model.h"

#include <QThread>

#include "base/thread_util.h"
#include "partman/partition_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/partition_util.h"

namespace installer {

PartitionModel::PartitionModel(QObject* parent)
    : QObject(parent),
      partition_manager_(new PartitionManager()),
      partition_thread_(new QThread(this)) {
  this->setObjectName("partition_model");

  partition_manager_->moveToThread(partition_thread_);
  partition_thread_->start();

  this->initConnections();
}

PartitionModel::~PartitionModel() {
  // Quit background thread explicitly.
  QuitThread(partition_thread_);
}

void PartitionModel::autoPart() {
  const QString script_path = GetAutoPartFile();
  emit partition_manager_->autoPart(script_path);
}

void PartitionModel::createPartitionTable(const QString& device_path) {
  // TODO(xushaohua): Umount any lvm physical partitions on |device_path|.
  // TODO(xushaohua): Check whether any partitions in |device_path| is busy.
  if (IsEfiEnabled()) {
    emit partition_manager_->createPartitionTable(device_path,
                                                  PartitionTableType::GPT);
  } else {
    emit partition_manager_->createPartitionTable(device_path,
                                                  PartitionTableType::MsDos);
  }
}

void PartitionModel::manualPart(const OperationList& operations) {
  emit partition_manager_->manualPart(operations);
}

void PartitionModel::scanDevices() {
  // If auto-part is not set, scan devices right now.
  if (!GetSettingsBool(kPartitionDoAutoPart)) {
#ifndef NDEBUG
    // Do not unmount any partitions in debug mode.
    const bool umount = false;
#else
    const bool umount = true;
#endif
    const bool enable_os_prober = GetSettingsBool(kPartitionEnableOsProber);
    emit partition_manager_->refreshDevices(umount, enable_os_prober);
  }
}

void PartitionModel::initConnections() {
  connect(partition_manager_, &PartitionManager::autoPartDone,
          this, &PartitionModel::autoPartDone);
  connect(partition_manager_, &PartitionManager::manualPartDone,
          this, &PartitionModel::manualPartDone);
  connect(partition_manager_, &PartitionManager::devicesRefreshed,
          this, &PartitionModel::deviceRefreshed);
}

}  // namespace installer