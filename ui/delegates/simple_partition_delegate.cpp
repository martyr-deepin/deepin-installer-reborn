// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/simple_partition_delegate.h"

namespace installer {

SimplePartitionDelegate::SimplePartitionDelegate(QObject* parent) : QObject(
    parent) {
  this->setObjectName("simple_partition_delegate");
}

}  // namespace installer