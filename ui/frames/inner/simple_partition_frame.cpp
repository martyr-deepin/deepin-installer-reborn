// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/simple_partition_frame.h"

#include <QLabel>

namespace ui {

SimplePartitionFrame::SimplePartitionFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("simple_partition_frame"));

  this->initUI();
  this->initConnections();
}

void SimplePartitionFrame::initConnections() {

}

void SimplePartitionFrame::initUI() {
  QLabel* label = new QLabel("Simple page", this);
  Q_UNUSED(label);
  this->setStyleSheet("background: red;");
}

}  // namespace ui