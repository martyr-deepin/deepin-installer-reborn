// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/advanced_partition_frame.h"

#include <QLabel>

#include "ui/frames/delegates/partition_delegate.h"

namespace ui {

AdvancedPartitionFrame::AdvancedPartitionFrame(
    PartitionDelegate* partition_delegate, QWidget* parent)
    : QFrame(parent),
      partition_delegate_(partition_delegate) {
  this->setObjectName(QStringLiteral("advanced_partition_frame"));

  this->initUI();
  this->initConnections();
}

void AdvancedPartitionFrame::initConnections() {

}

void AdvancedPartitionFrame::initUI() {
  QLabel* label = new QLabel("Advanced page", this);
  Q_UNUSED(label);
  this->setStyleSheet("background: white;");
}

}  // namespace ui