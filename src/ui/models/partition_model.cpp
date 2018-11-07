/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
