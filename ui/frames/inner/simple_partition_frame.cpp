// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/simple_partition_frame.h"

#include <QButtonGroup>
#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>

#include "service/partition_manager_structs.h"
#include "ui/frames/delegates/partition_delegate.h"
#include "ui/widgets/simple_partition_button.h"

namespace ui {

SimplePartitionFrame::SimplePartitionFrame(
    PartitionDelegate* partition_delegate, QWidget* parent)
    : QFrame(parent),
      partition_delegate_(partition_delegate) {
  this->setObjectName(QStringLiteral("simple_partition_frame"));

  this->initUI();
  this->initConnections();
}

void SimplePartitionFrame::initConnections() {
  connect(partition_delegate_, &PartitionDelegate::deviceRefreshed,
          this, &SimplePartitionFrame::onDeviceRefreshed);
}

void SimplePartitionFrame::initUI() {
}

void SimplePartitionFrame::onDeviceRefreshed() {
  qDebug() << "simple-partition-frame::on device refreshed";
  // Draw partitions.
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignCenter);
  QButtonGroup* button_group = new QButtonGroup(this);
  for (const service::Device& device : partition_delegate_->devices) {
    qDebug() << "=======================";
    qDebug() << device.model;
    QGridLayout* grid_layout = new QGridLayout();
    grid_layout->setHorizontalSpacing(20);
    grid_layout->setVerticalSpacing(20);
    layout->addLayout(grid_layout);
    int row = 0, column = 0;

    qDebug() << "partition size:" << device.partitions.length();
    for (const service::Partition& partition : device.partitions) {
      qDebug() << "partition:" << partition.path;
      if (partition.type != service::PartitionType::Normal &&
          partition.type != service::PartitionType::Logical &&
          partition.type != service::PartitionType::Freespace) {
        continue;
      }
      SimplePartitionButton* button = new SimplePartitionButton(partition);
      button_group->addButton(button);
      grid_layout->addWidget(button, row, column);
      qDebug() << "add button:" << row << column;

      column ++;
      // Add rows.
      if (column > 3) {
        column = 0;
        row ++;
      }
    }
  }

  this->setLayout(layout);
}

}  // namespace ui