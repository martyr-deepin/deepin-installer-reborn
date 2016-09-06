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
#include "partman/structs.h"
#include "ui/frames/delegates/partition_delegate.h"
#include "ui/widgets/simple_partition_button.h"

namespace ui {

namespace {

// 4 partitions are displays at each row.
const int kPartitionColumns = 4;

}

SimplePartitionFrame::SimplePartitionFrame(
    PartitionDelegate* partition_delegate, QWidget* parent)
    : QScrollArea(parent),
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
  tip_layout->addWidget(tip_label);
  tip_layout->addStretch();

  install_tip_ = new QFrame(this);
  // Same width as SimplePartitionButton.
  // TODO(xushaohua): Add a const variable.
  install_tip_->setFixedWidth(220);
  install_tip_->setLayout(tip_layout);
  install_tip_->hide();

//  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//  this->setSizeAdjustPolicy(Q)
  this->ensureVisible(0, 0, 640, 480);
  this->setWidgetResizable(true);
  this->setStyleSheet(
      base::ReadTextFileContent(":/styles/simple_partition_frame.css"));
}

void SimplePartitionFrame::drawDevices() {
  // Draw partitions.

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignCenter);
  for (const partman::Device& device : partition_delegate_->devices()) {
    QLabel* device_model_label = new QLabel(device.model);
    device_model_label->setObjectName("device_model");

    QGridLayout* grid_layout = new QGridLayout();
    grid_layout->setHorizontalSpacing(20);
    grid_layout->setVerticalSpacing(20);
    // Make sure that widgets in grid are left-aligned.
    grid_layout->setColumnStretch(kPartitionColumns, 1);
    layout->addWidget(device_model_label);
    layout->addLayout(grid_layout);
    int row = 0, column = 0;

    for (const partman::Partition& partition : device.partitions) {
      SimplePartitionButton* button = new SimplePartitionButton(partition);
      partition_button_group_->addButton(button);
      grid_layout->addWidget(button, row, column, Qt::AlignLeft);

      column ++;
      // Add rows.
      if (column >= kPartitionColumns) {
        column = 0;
        row ++;
      }
    }
  }

  QFrame* wrapper = new QFrame();
  wrapper->setLayout(layout);
  this->setWidget(wrapper);
}

void SimplePartitionFrame::onDeviceRefreshed() {
  this->drawDevices();
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