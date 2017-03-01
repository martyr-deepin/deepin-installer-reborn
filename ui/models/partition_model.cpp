// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/partition_model.h"

#include <QThread>

#include "base/thread_util.h"
#include "partman/partition_manager.h"

namespace installer {

PartitionModel::PartitionModel(QObject* parent)
    : QObject(parent),
      partition_manager_(new PartitionManager()),
      partition_thread_(new QThread(this)) {
  this->setObjectName("partition_model");

  partition_manager_->moveToThread(partition_thread_);
  partition_thread_->start();
}

PartitionModel::~PartitionModel() {
  // Quit background thread explicitly.
  QuitThread(partition_thread_);
}

void PartitionModel::autoPart() {

}

void PartitionModel::createPartitionTable(const QString& device_path) {
  Q_UNUSED(device_path);
}

void PartitionModel::scanDevices() {

}

}  // namespace installer