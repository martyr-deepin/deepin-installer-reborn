// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/advanced_partition_frame.h"

#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#include "ui/frames/delegates/partition_delegate.h"
#include "ui/widgets/advanced_partition_item.h"

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
  connect(partition_delegate_, &PartitionDelegate::deviceRefreshed,
          this, &AdvancedPartitionFrame::onDeviceRefreshed);
}

void AdvancedPartitionFrame::initUI() {
  partition_layout_ = new QVBoxLayout();

  QScrollArea* main_area = new QScrollArea();
  main_area->setLayout(partition_layout_);

  QHBoxLayout* bottom_layout = new QHBoxLayout();

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(main_area);
  layout->addLayout(bottom_layout);

  this->setLayout(layout);
}

void AdvancedPartitionFrame::onDeviceRefreshed() {
  qDebug() << "advanced partition frame: on device refreshed()";

  for (const partman::Device& device : partition_delegate_->devices) {
    qDebug() << "=======================";
    qDebug() << device.model;
    QLabel* model_label = new QLabel(device.model);
    partition_layout_->addWidget(model_label);
    for (const partman::Partition& partition : device.partitions) {
      qDebug() << "partition:" << partition.path;
      if ((partition.type != partman::PartitionType::Primary) &&
          (partition.type != partman::PartitionType::Logical) &&
          (partition.type != partman::PartitionType::Unallocated)) {
        continue;
      }

      // Filters freespace partition based on size.
      if (partition.type == partman::PartitionType::Unallocated &&
          partition.length < kMinimumPartitionSizeToDisplay) {
        continue;
      }

      AdvancedPartitionItem* item = new AdvancedPartitionItem(partition);
      partition_layout_->addWidget(item);
    }
  }
}

}  // namespace ui