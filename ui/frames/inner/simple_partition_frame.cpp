// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/simple_partition_frame.h"

#include <QButtonGroup>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "base/file_util.h"
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
  connect(partition_button_group_,
          static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>
          (&QButtonGroup::buttonToggled),
          this, &SimplePartitionFrame::onPartitionButtonToggled);
}

void SimplePartitionFrame::initUI() {
  partition_button_group_ = new QButtonGroup(this);

  QHBoxLayout* tip_layout = new QHBoxLayout();
  QLabel* tip_label = new QLabel(tr("Install here"));
  tip_label->setObjectName("tip_label");
  tip_label->setAlignment(Qt::AlignCenter);

  tip_layout->addStretch();
  // TODO(xushaohua): Add an icon.
  tip_layout->addWidget(tip_label);
  tip_layout->addStretch();

  install_tip_ = new QFrame(this);
  // Same width as SimplePartitionButton.
  install_tip_->setFixedWidth(220);
  install_tip_->setLayout(tip_layout);
  install_tip_->hide();
  install_tip_->setStyleSheet(
      base::ReadTextFileContent(":/styles/simple_partition_install_tip.css"));
}

void SimplePartitionFrame::onDeviceRefreshed() {
  qDebug() << "simple-partition-frame::on device refreshed";
  // Draw partitions.
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignCenter);
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
      if ((partition.type != service::PartitionType::Normal) &&
          (partition.type != service::PartitionType::Logical) &&
          (partition.type != service::PartitionType::Freespace)) {
        continue;
      }

      // Filters freespace partition based on size.
      if (partition.type == service::PartitionType::Freespace &&
          partition.length < kMinimumPartitionSizeToDisplay) {
        continue;
      }

      SimplePartitionButton* button = new SimplePartitionButton(partition);
      partition_button_group_->addButton(button);
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

void SimplePartitionFrame::onPartitionButtonToggled(QAbstractButton* button,
                                                    bool checked) {
  if (checked) {
    qDebug() << "pos:" << button->pos() << ",size:" << button->size();
    const QPoint pos = button->pos();
    const QSize size = button->size();
    install_tip_->move(pos.x(), pos.y() + size.height());
    install_tip_->show();
  }
}

}  // namespace ui