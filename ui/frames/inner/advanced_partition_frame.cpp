// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/advanced_partition_frame.h"

#include <QButtonGroup>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QtCore/QEvent>

#include "base/file_util.h"
#include "partman/utils.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/advanced_partition_button.h"
#include "ui/widgets/pointer_button.h"

namespace installer {

namespace {

const int kWindowWidth = 960;

const char kTextBootloader[] = "Select bootloader";
const char kTextDelete[] = "Delete";
const char kTextDone[] = "Done";

}  // namespace

AdvancedPartitionFrame::AdvancedPartitionFrame(PartitionDelegate* delegate_,
                                               QWidget* parent)
    : QFrame(parent),
      delegate_(delegate_) {
  this->setObjectName("advanced_partition_frame");

  this->initUI();
  this->initConnections();
}

bool AdvancedPartitionFrame::validate() {
  bool efi_is_set = false;
  bool root_is_set = false;
  bool swap_is_set = false;
  for (const Device& device : delegate_->devices()) {
    for (const Partition& partition : device.partitions) {
      if (partition.mount_point == kMountPointRoot) {
        root_is_set = true;
      } else if (partition.fs == FsType::EFI) {
        efi_is_set = true;
      } else if (partition.fs == FsType::LinuxSwap) {
        swap_is_set = true;
      }
    }
  }

  if (!root_is_set) {
    // Notify that root is not set.
    msg_label_->setText(tr("A root partition is required"));
    return false;
  }

  if (!swap_is_set) {
    // Notify that linux-swap partition is not set.
    msg_label_->setText(tr("A swap partition is required"));
    return false;
  }

  if (IsEfiEnabled() && !efi_is_set) {
    // Notify that EFI partition is not set.
    msg_label_->setText(tr("An EFI partition is required"));
    return false;
  }

  msg_label_->clear();
  return true;
}

void AdvancedPartitionFrame::setBootloaderPath(const QString& bootloader_path) {
  bootloader_button_->setText(bootloader_path);
}

void AdvancedPartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    bootloader_label_->setText(tr(kTextBootloader));
    if (editing_button_->isChecked()) {
      editing_button_->setText(tr(kTextDone));
    } else {
      editing_button_->setText(tr(kTextDelete));
    }
  } else {
    QFrame::changeEvent(event);
  }
}

void AdvancedPartitionFrame::initConnections() {
  connect(delegate_, &PartitionDelegate::deviceRefreshed,
          this, &AdvancedPartitionFrame::onDeviceRefreshed);
  connect(bootloader_button_, &QPushButton::clicked,
          this, &AdvancedPartitionFrame::requestSelectBootloaderFrame);
  connect(editing_button_, &QPushButton::toggled,
          this, &AdvancedPartitionFrame::onEditButtonToggled);

  // Clear content in msg_label when NewPartitionFrame or EditPartitionFrame is
  // raised.
  connect(this, &AdvancedPartitionFrame::requestEditPartitionFrame,
          msg_label_, &QLabel::clear);
  connect(this, &AdvancedPartitionFrame::requestNewPartitionFrame,
          msg_label_, &QLabel::clear);
}

void AdvancedPartitionFrame::initUI() {
  partition_button_group_ = new QButtonGroup(this);

  partition_layout_ = new QVBoxLayout();
  partition_layout_->setContentsMargins(0, 0, 0, 0);
  partition_layout_->setSpacing(0);
  partition_layout_->setSizeConstraint(QLayout::SetMinAndMaxSize);

  QFrame* partition_list_frame = new QFrame();
  partition_list_frame->setObjectName("partition_list_frame");
  partition_list_frame->setContentsMargins(0, 0, 0, 0);
  partition_list_frame->setLayout(partition_layout_);
  partition_list_frame->setFixedWidth(kWindowWidth);

  QScrollArea* scroll_area = new QScrollArea();
  QSizePolicy scroll_area_size_policy(QSizePolicy::Preferred,
                                      QSizePolicy::MinimumExpanding);
  scroll_area->setContentsMargins(0, 0, 0, 0);
  scroll_area_size_policy.setHorizontalStretch(10);
  scroll_area_size_policy.setVerticalStretch(10);
  scroll_area->setSizePolicy(scroll_area_size_policy);
  scroll_area->setWidget(partition_list_frame);
  scroll_area->setWidgetResizable(true);
  scroll_area->setFixedWidth(kWindowWidth);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  bootloader_label_ = new QLabel(tr(kTextBootloader), this);
  bootloader_label_->setObjectName("bootloader_label");
  bootloader_button_ = new PointerButton(this);
  bootloader_button_->setObjectName("bootloader_button");
  bootloader_button_->setFlat(true);

  editing_button_ = new PointerButton(tr(kTextDelete));
  editing_button_->setFlat(true);
  editing_button_->setObjectName("editing_button");
  editing_button_->setCheckable(true);
  editing_button_->setChecked(false);

  QHBoxLayout* bottom_layout = new QHBoxLayout();
  bottom_layout->setContentsMargins(15, 10, 15, 10);
  bottom_layout->setSpacing(10);
  // Show bootloader button only if EFI mode is off.
  if (!IsEfiEnabled()) {
    bottom_layout->addWidget(bootloader_label_);
    bottom_layout->addWidget(bootloader_button_);
  }
  bottom_layout->addStretch();
  bottom_layout->addWidget(editing_button_);

  QFrame* bottom_frame = new QFrame();
  bottom_frame->setObjectName("bottom_frame");
  bottom_frame->setContentsMargins(0, 0, 0, 0);
  bottom_frame->setLayout(bottom_layout);
  bottom_frame->setFixedWidth(kWindowWidth);

  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->addWidget(scroll_area, 1, Qt::AlignHCenter);
  main_layout->addSpacing(1);
  main_layout->addWidget(bottom_frame, 0, Qt::AlignHCenter);

  QFrame* main_frame = new QFrame();
  main_frame->setObjectName("main_frame");
  main_frame->setContentsMargins(0, 0, 0, 0);
  main_frame->setLayout(main_layout);

  msg_label_ = new QLabel();
  msg_label_->setObjectName("msg_label");
  msg_label_->setFixedHeight(20);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(main_frame, 0, Qt::AlignHCenter);
  layout->addSpacing(20);
  layout->addWidget(msg_label_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  QSizePolicy container_policy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  container_policy.setVerticalStretch(100);
  this->setSizePolicy(container_policy);
  this->setStyleSheet(ReadFile(":/styles/advanced_partition_frame.css"));
}

void AdvancedPartitionFrame::repaintDevices() {
  qDebug() << "AdvancedPartitionFrame::repaintDevices()";
  // Clear children in button group.
  for (QAbstractButton* button : partition_button_group_->buttons()) {
    partition_button_group_->removeButton(button);
  }

  // Remove all widgets in partition layout.
  for (QLayoutItem* item = partition_layout_->takeAt(0); item != NULL;
       item = partition_layout_->takeAt(0)) {
    delete item->widget();
    delete item;
  }

  for (const Device& device : delegate_->devices()) {
    QLabel* model_label = new QLabel();
    model_label->setObjectName("model_label");
    model_label->setText(GetDeviceModelAndCap(device));
    model_label->setContentsMargins(15, 10, 0, 5);
    partition_layout_->addWidget(model_label, 0, Qt::AlignLeft);
    for (const Partition& partition : device.partitions) {
      if (partition.type == PartitionType::Extended) {
        // Ignores extended partition.
        continue;
      }
      AdvancedPartitionButton* button = new AdvancedPartitionButton(partition);
      button->setEditable(editing_button_->isChecked());
      partition_layout_->addWidget(button);
      partition_button_group_->addButton(button);
      button->show();

      connect(editing_button_, &QPushButton::toggled,
              button, &AdvancedPartitionButton::setEditable);
      connect(button, &AdvancedPartitionButton::editPartitionTriggered,
              this, &AdvancedPartitionFrame::requestEditPartitionFrame);
      connect(button, &AdvancedPartitionButton::newPartitionTriggered,
              this, &AdvancedPartitionFrame::requestNewPartitionFrame);
      connect(button, &AdvancedPartitionButton::deletePartitionTriggered,
              delegate_, &PartitionDelegate::deletePartition);
    }
  }

  // Add stretch to expand vertically
  partition_layout_->addStretch();
}

void AdvancedPartitionFrame::onDeviceRefreshed() {
  this->repaintDevices();
}

void AdvancedPartitionFrame::onEditButtonToggled(bool toggle) {
  if (toggle) {
    editing_button_->setText(tr(kTextDone));
  } else {
    editing_button_->setText(tr(kTextDelete));
  }
}

}  // namespace installer
