// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/partition_model.h"

namespace installer {

PartitionModel::PartitionModel(QObject* parent) : QObject(parent) {
  this->setObjectName("partition_model");
}

}  // namespace installer