// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/simple_disk_frame.h"

#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

#include "base/file_util.h"
#include "ui/delegates/simple_partition_delegate.h"
#include "ui/widgets/simple_disk_button.h"
#include "ui/utils/widget_util.h"

namespace installer {

namespace {

// 4 partitions are displays at each row.
const int kDiskColumns = 4;

const int kWindowWidth = 960;

}  // namespace

SimpleDiskFrame::SimpleDiskFrame(
    SimplePartitionDelegate* delegate,
    QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName("simple_disk_frame");

  this->initUI();
  this->initConnections();
}

void SimpleDiskFrame::initConnections() {
  // Repaint layout only when real device list is updated.
  connect(delegate_, &SimplePartitionDelegate::deviceRefreshed,
          this, &SimpleDiskFrame::onDeviceRefreshed);
  connect(button_group_,
          static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>
          (&QButtonGroup::buttonToggled),
          this, &SimpleDiskFrame::onPartitionButtonToggled);
}

void SimpleDiskFrame::initUI() {
  button_group_ = new QButtonGroup(this);

  QLabel* tip_icon = new QLabel();
  tip_icon->setPixmap(QPixmap(":/images/install_icon.png"));
  tip_label_ = new QLabel(tr("Install here"));
  tip_label_->setObjectName("tip_label");
  tip_label_->setFixedHeight(18);

  QHBoxLayout* tip_layout = new QHBoxLayout();
  tip_layout->setContentsMargins(0, 0, 0, 0);
  tip_layout->setSpacing(0);
  tip_layout->addStretch();
  tip_layout->addWidget(tip_icon, 0, Qt::AlignVCenter);
  tip_layout->addWidget(tip_label_, 0, Qt::AlignVCenter);
  tip_layout->addStretch();

  install_tip_ = new QFrame();
  install_tip_->setObjectName("install_tip");
  install_tip_->setContentsMargins(0, 0, 0, 0);
  // Same width as SimplePartitionButton.
  install_tip_->setFixedWidth(220);
  install_tip_->setLayout(tip_layout);
  install_tip_->hide();

  grid_layout_ = new QGridLayout();
  grid_layout_->setSpacing(0);
  grid_layout_->setContentsMargins(0, 0, 0, 0);
  grid_layout_->setHorizontalSpacing(20);
  grid_layout_->setVerticalSpacing(20);
  grid_layout_->setColumnStretch(kDiskColumns, 1);

  grid_wrapper_ = new QFrame();
  grid_wrapper_->setObjectName("grid_wrapper");
  grid_wrapper_->setLayout(grid_layout_);
  install_tip_->setParent(grid_wrapper_);

  QScrollArea* scroll_area = new QScrollArea();
  scroll_area->setObjectName("scroll_area");
  scroll_area->setWidget(grid_wrapper_);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area->setWidgetResizable(true);

  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->addWidget(scroll_area, Qt::AlignHCenter);
  main_layout->addStretch();
  main_layout->addSpacing(10);

  this->setLayout(main_layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setFixedWidth(kWindowWidth);
  QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  policy.setVerticalStretch(100);
  this->setSizePolicy(policy);
  this->setStyleSheet(ReadFile(":/styles/simple_disk_frame.css"));
}

void SimpleDiskFrame::repaintDevices() {
  // Clear grid layout.
  ClearLayout(grid_layout_);

  // Clear button group.
  for (QAbstractButton* button : button_group_->buttons()) {
    button_group_->removeButton(button);
  }

  // Hide tooltip frame
  install_tip_->hide();

  // Draw partitions.
  int row = 0, column = 0;
  for (const Device& device : delegate_->virtual_devices()) {
    SimpleDiskButton* button = new SimpleDiskButton(device);

    button_group_->addButton(button);
    grid_layout_->addWidget(button, row, column, Qt::AlignHCenter);

    connect(button, &QPushButton::clicked,
            this, &SimpleDiskFrame::onPartitionButtonClicked);

    column += 1;
    // Add rows.
    if (column >= kDiskColumns) {
      column = 0;
      row += 1 ;
    }
  }

  // Add place holder. It is used for install_tip
  row += 1;
  QLabel* place_holder_label = new QLabel(this);
  place_holder_label->setObjectName("place_holder_label");
  place_holder_label->setFixedSize(kWindowWidth, 30);
  grid_layout_->addWidget(place_holder_label, row, 0,
                          1, kDiskColumns, Qt::AlignHCenter);

  // Resize grid_wrapper explicitly.
  grid_wrapper_->adjustSize();
}

void SimpleDiskFrame::onDeviceRefreshed() {
  this->repaintDevices();
}

void SimpleDiskFrame::onPartitionButtonToggled() {

}

void SimpleDiskFrame::onPartitionButtonClicked() {

}

}  // namespace installer