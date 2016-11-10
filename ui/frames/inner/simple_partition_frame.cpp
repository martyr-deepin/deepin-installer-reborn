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
#include <QtCore/QEvent>

#include "base/file_util.h"
#include "partman/structs.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/device_model_label.h"
#include "ui/widgets/simple_partition_button.h"

namespace installer {

namespace {

// 4 partitions are displays at each row.
const int kPartitionColumns = 4;

const int kWindowWidth = 960;

const char kTextTip[] = "Install here";

}  // namespace

SimplePartitionFrame::SimplePartitionFrame(PartitionDelegate* delegate,
                                           QWidget* parent)
    : QScrollArea(parent),
      delegate_(delegate) {
  this->setObjectName("simple_partition_frame");

  this->initUI();
  this->initConnections();
}

void SimplePartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    tip_label_->setText(tr(kTextTip));
  } else {
    QFrame::changeEvent(event);
  }
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

  QLabel* tip_icon = new QLabel();
  tip_icon->setPixmap(QPixmap(":/images/install_icon.png"));

  tip_label_ = new QLabel(tr(kTextTip));
  tip_label_->setObjectName("tip_label");
  tip_label_->setFixedHeight(18);

  QHBoxLayout* tip_layout = new QHBoxLayout();
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
  grid_layout_->setColumnStretch(kPartitionColumns, 1);

  QFrame* grid_wrapper = new QFrame();
  grid_wrapper->setObjectName("grid_wrapper");
  grid_wrapper->setLayout(grid_layout_);
  install_tip_->setParent(grid_wrapper);
  this->setWidget(grid_wrapper);

  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setWidgetResizable(true);
  this->setFixedWidth(kWindowWidth);
  QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  policy.setVerticalStretch(100);
  this->setSizePolicy(policy);
  this->setStyleSheet(ReadFile(":/styles/simple_partition_frame.css"));
}

void SimplePartitionFrame::repaintDevices() {
  // Clear grid layout.
  QLayoutItem* layout_item;
  while ((layout_item = grid_layout_->takeAt(0)) != nullptr) {
    delete layout_item->widget();
    delete layout_item;
    layout_item = nullptr;
  }

  // Clear button group.
  for (QAbstractButton* button : button_group_->buttons()) {
    button_group_->removeButton(button);
  }

  // Draw partitions.
  int row = 0, column = 0;
  for (const Device& device : delegate_->devices()) {
    DeviceModelLabel* device_model_label = new DeviceModelLabel();
    device_model_label->setText(GetDeviceModelAndCap(device));
    device_model_label->setFixedSize(kWindowWidth, 20);

    // Make sure that widgets in grid are left-aligned.
    grid_layout_->addWidget(device_model_label, row, 0,
                            1, kPartitionColumns, Qt::AlignLeft);
    row += 1;

    for (const Partition& partition : device.partitions) {
      if (partition.type == PartitionType::Extended) {
        // Ignores extended partition.
        continue;
      }
      SimplePartitionButton* button = new SimplePartitionButton(partition);
      button_group_->addButton(button);
      grid_layout_->addWidget(button, row, column, Qt::AlignHCenter);
      // TODO(xushaohua): Select root partition

      column += 1;
      // Add rows.
      if (column >= kPartitionColumns) {
        column = 0;
        row += 1 ;
      }
    }
    // Go to next row.
    column = 0;
    row += 1;
  }

  // Add place holder. It is used for install_tip
  row += 1;
  QLabel* place_holder_label = new QLabel();
  place_holder_label->setObjectName("place_holder_label");
  place_holder_label->setFixedSize(kWindowWidth, 30);
  grid_layout_->addWidget(place_holder_label, row, 0,
                          1, kPartitionColumns, Qt::AlignHCenter);
}

void SimplePartitionFrame::onDeviceRefreshed() {
  this->repaintDevices();
}

void SimplePartitionFrame::onPartitionButtonToggled(QAbstractButton* button,
                                                    bool checked) {
  if (checked) {
    // Move install_tip to bottom of button
    const QPoint pos = button->pos();
    install_tip_->move(pos.x(), pos.y() - 10);
    install_tip_->show();
  }

  SimplePartitionButton* part_button =
      qobject_cast<SimplePartitionButton*>(button);
  if (part_button) {
    // Clear mount point, no matter button is checked or not.
//      delegate_->updateMountPoint(part_button->partition(), "");
    // NOTE(xushaohua): Call updateMountPoint() asynchronously,
    // or else it crashes.
    this->metaObject()->invokeMethod(delegate_, "updateMountPoint",
                                     Qt::QueuedConnection,
                                     Q_ARG(Partition, part_button->partition()),
                                     Q_ARG(QString, ""));
  }
}

}  // namespace installer