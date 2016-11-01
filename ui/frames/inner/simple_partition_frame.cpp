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
#include "ui/delegates/partition_delegate.h"
#include "ui/widgets/device_model_label.h"
#include "ui/widgets/simple_partition_button.h"

namespace installer {

namespace {

// 4 partitions are displays at each row.
const int kPartitionColumns = 4;

const int kWindowWidth = 960;

}  // namespace

SimplePartitionFrame::SimplePartitionFrame(PartitionDelegate* delegate,
                                           QWidget* parent)
    : QScrollArea(parent),
      delegate_(delegate) {
  this->setObjectName(QStringLiteral("simple_partition_frame"));

  this->initUI();
  this->initConnections();
}

void SimplePartitionFrame::initConnections() {
  connect(delegate_, &PartitionDelegate::deviceRefreshed,
          this, &SimplePartitionFrame::onDeviceRefreshed);
  connect(button_group_,
          static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>
          (&QButtonGroup::buttonToggled),
          this, &SimplePartitionFrame::onPartitionButtonToggled);
}

void SimplePartitionFrame::initUI() {
  button_group_ = new QButtonGroup(this);

//  QHBoxLayout* tip_layout = new QHBoxLayout();
//  QLabel* tip_label = new QLabel(tr("Install here"));
//  tip_label->setObjectName("tip_label");
//  tip_label->setAlignment(Qt::AlignCenter);
//
//  tip_layout->addStretch();
//  tip_layout->addWidget(tip_label);
//  tip_layout->addStretch();

//  install_tip_ = new QFrame(this);
//  // Same width as SimplePartitionButton.
//  install_tip_->setFixedWidth(220);
//  install_tip_->setLayout(tip_layout);
//  install_tip_->hide();

  grid_layout_ = new QGridLayout();
  grid_layout_->setSpacing(0);
  grid_layout_->setContentsMargins(0, 0, 0, 0);
  grid_layout_->setHorizontalSpacing(20);
  grid_layout_->setVerticalSpacing(20);
  grid_layout_->setColumnStretch(kPartitionColumns, 1);

  QFrame* grid_wrapper = new QFrame();
  grid_wrapper->setObjectName("grid_wrapper");
  grid_wrapper->setLayout(grid_layout_);
  this->setWidget(grid_wrapper);

  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setWidgetResizable(true);
  this->setFixedWidth(kWindowWidth);
  this->setFixedHeight(640);
  QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
  policy.setVerticalStretch(100);
  this->setSizePolicy(policy);
  this->setStyleSheet(
      ReadTextFileContent(":/styles/simple_partition_frame.css"));
}

void SimplePartitionFrame::repaintDevices() {
  // Clear grid layout.
  QLayoutItem* layout_item;
  while ((layout_item = grid_layout_->takeAt(0)) != nullptr) {
    delete layout_item;
    layout_item = nullptr;
  }

  // Draw partitions.
  int row = 0, column = 0;
  for (const Device& device : delegate_->devices()) {
    QLabel* device_model_label = new DeviceModelLabel(device.model);
    device_model_label->setFixedSize(kWindowWidth, 20);
    device_model_label->setObjectName("device_model");

    // Make sure that widgets in grid are left-aligned.
    grid_layout_->addWidget(device_model_label, row, 0,
                            1, kPartitionColumns, Qt::AlignLeft);
    row ++;

    for (const Partition& partition : device.partitions) {
      if (partition.type == PartitionType::Extended) {
        // Ignores extended partition.
        continue;
      }
      SimplePartitionButton* button = new SimplePartitionButton(partition);
      button_group_->addButton(button);
      grid_layout_->addWidget(button, row, column, Qt::AlignHCenter);

      column ++;
      // Add rows.
      if (column >= kPartitionColumns) {
        column = 0;
        row ++;
      }
    }
    // Go to next row.
    column = 0;
    row ++;
  }
}

void SimplePartitionFrame::onDeviceRefreshed() {
  this->repaintDevices();
}

void SimplePartitionFrame::onPartitionButtonToggled(QAbstractButton* button,
                                                    bool checked) {
  if (checked) {
    qDebug() << "pos:" << button->pos() << ",size:" << button->size();
//    const QPoint pos = button->pos();
//    const QSize size = button->size();
//    install_tip_->move(pos.x(), pos.y() + size.height());
//    install_tip_->show();
  }
}

}  // namespace installer