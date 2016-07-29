// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/advanced_partition_frame.h"

namespace ui {

AdvancedPartitionFrame::AdvancedPartitionFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName(QStringLiteral("advanced_partition_frame"));

  this->initUI();
  this->initConnections();
}

void AdvancedPartitionFrame::initConnections() {

}

void AdvancedPartitionFrame::initUI() {

}

}  // namespace ui