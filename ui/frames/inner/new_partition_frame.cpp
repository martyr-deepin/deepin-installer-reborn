// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/new_partition_frame.h"

namespace ui {

NewPartitionFrame::NewPartitionFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("new_partition_frame"));

  this->initUI();
  this->initConnections();
}

void NewPartitionFrame::initConnections() {

}

void NewPartitionFrame::initUI() {

}

}  // namespace ui