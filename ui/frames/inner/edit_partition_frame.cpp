// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/edit_partition_frame.h"

namespace ui {

EditPartitionFrame::EditPartitionFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("edit_partition_frame"));

  this->initUI();
  this->initConnections();
}

void EditPartitionFrame::initConnections() {

}

void EditPartitionFrame::initUI() {

}

}  // namespace ui