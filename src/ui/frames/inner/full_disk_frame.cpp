/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui/frames/inner/full_disk_frame.h"

#include <QButtonGroup>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

#include "base/file_util.h"
#include "ui/delegates/full_disk_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/simple_disk_button.h"
#include "ui/utils/widget_util.h"

namespace installer {

namespace {

// 4 partitions are displays at each row.
const int kDiskColumns = 4;

const int kWindowWidth = 960;

}  // namespace

FullDiskFrame::FullDiskFrame(FullDiskDelegate* delegate, QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName("simple_disk_frame");

  this->initUI();
  this->initConnections();
}

FullDiskFrame::~FullDiskFrame() {

}

bool FullDiskFrame::validate() const {
  return (button_group_->checkedButton() != nullptr);
}

void FullDiskFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    tip_label_->setText(tr("Install here"));
  } else {
    QFrame::changeEvent(event);
  }
}

void FullDiskFrame::initConnections() {
  // Repaint layout only when real device list is updated.
  connect(delegate_, &FullDiskDelegate::deviceRefreshed,
          this, &FullDiskFrame::onDeviceRefreshed);
  connect(button_group_,
          static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>
          (&QButtonGroup::buttonToggled),
          this, &FullDiskFrame::onPartitionButtonToggled);
}

void FullDiskFrame::initUI() {
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

void FullDiskFrame::repaintDevices() {
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
    bool partition_busy = false;
    for (const Partition& partition : device.partitions) {
      if (partition.busy) {
        partition_busy = true;
        break;
      }
    }
    if (partition_busy) {
      qWarning() << "Partition on device is busy!" << device.path;
      continue;
    }
    SimpleDiskButton* button = new SimpleDiskButton(device);

    button_group_->addButton(button);
    grid_layout_->addWidget(button, row, column, Qt::AlignHCenter);

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

void FullDiskFrame::showInstallTip(QAbstractButton* button) {
  // Move install_tip to bottom of button
  const QPoint pos = button->pos();
  install_tip_->move(pos.x(), pos.y() - 10);
  install_tip_->show();
}

void FullDiskFrame::onDeviceRefreshed() {
  this->repaintDevices();
}

void FullDiskFrame::onPartitionButtonToggled(QAbstractButton* button,
                                             bool checked) {
  SimpleDiskButton* part_button = dynamic_cast<SimpleDiskButton*>(button);
  if (!part_button) {
    qCritical() << "no disk button is selected";
    return;
  }

  if (!checked) {
    // Deselect previous button.
    part_button->setSelected(false);
  } else {
    // Hide tooltip.
    install_tip_->hide();

    const QString path = part_button->device().path;
    qDebug() << "selected device path:" << path;
    part_button->setSelected(true);

    // Show install-tip at bottom of current checked button.
    this->showInstallTip(part_button);

    // Reset simple operations.
    delegate_->resetOperations();

    PartitionTableType table = IsEfiEnabled() ?
                               PartitionTableType::GPT :
                               PartitionTableType::MsDos;
    delegate_->formatWholeDevice(path, table);
  }
}

}  // namespace installer