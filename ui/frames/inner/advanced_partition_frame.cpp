// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/advanced_partition_frame.h"

#include <QButtonGroup>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

#include "base/file_util.h"
#include "partman/utils.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/advanced_partition_button.h"
#include "ui/utils/widget_util.h"

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
  bool efi_large_enough = false;
  bool root_is_set = false;
  bool root_large_enough = false;
  bool swap_is_set = false;
  bool boot_is_set = false;
  bool boot_large_enough = false;

  const int root_required = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
  const int boot_recommended = GetSettingsInt(kPartitionDefaultBootSpace);
  const int efi_recommended = GetSettingsInt(kPartitionDefaultEFISpace);

  for (const Device& device : delegate_->devices()) {
    for (const Partition& partition : device.partitions) {
      if (partition.mount_point == kMountPointRoot) {
        // Check / partition.
        root_is_set = true;
        const qint64 minimum_bytes = root_required * kGibiByte;
        root_large_enough =  (partition.getByteLength() > minimum_bytes);

      } else if (partition.mount_point == kMountPointBoot) {
        // Check /boot partition.
        boot_is_set = true;
        const qint64 recommend_bytes = boot_recommended * kMebiByte;
        boot_large_enough = (partition.getByteLength() > recommend_bytes);

      } else if (partition.fs == FsType::EFI) {
        // Check EFI partition.
        efi_is_set = true;

        const qint64 recommended_bytes = efi_recommended * kMebiByte;
        efi_large_enough = (partition.getByteLength() > recommended_bytes);
      } else if (partition.fs == FsType::LinuxSwap) {
        swap_is_set = true;
      }
    }
  }

  if (!root_is_set) {
    msg_label_->setText(tr("A root partition is required"));
    return false;
  }

  if (!root_large_enough) {
    msg_label_->setText(
        tr("At least %1 Gib is required for root partition.")
            .arg(root_required));
    return false;
  }

  if (!swap_is_set) {
    if (IsSwapAreaNeeded()) {
      // In advanced-mode, notify user to create linux-swap partition, instead
      // of creating swap-file automatically. Because swap partition is more
      // stable and faster than swap file.
      msg_label_->setText(tr("A linux swap partition is required"));
      return false;
    }
  }

  if (IsEfiEnabled()) {
    if (!efi_is_set) {
      msg_label_->setText(tr("An EFI partition is required"));
      return false;
    }
    if (!efi_large_enough) {
      msg_label_->setText(
          tr("At least %1 Mib is required for EFI partition.")
              .arg(efi_recommended));
      return false;
    }
  }

  if (boot_is_set) {
    if (!boot_large_enough) {
      msg_label_->setText(
          tr("At least %1 Mib is required for /boot partition.")
              .arg(boot_recommended));
      return false;
    }
  }

  msg_label_->clear();
  return true;
}

void AdvancedPartitionFrame::setBootloaderPath(const QString& bootloader_path) {
  bootloader_button_->setText(bootloader_path);
}

void AdvancedPartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    bootloader_tip_button_->setText(tr(kTextBootloader));
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
  connect(bootloader_tip_button_, &QPushButton::clicked,
          this, &AdvancedPartitionFrame::requestSelectBootloaderFrame);
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
  scroll_area->setContentsMargins(0, 0, 0, 0);
  QSizePolicy scroll_area_size_policy(QSizePolicy::Fixed,
                                      QSizePolicy::MinimumExpanding);
  scroll_area_size_policy.setHorizontalStretch(10);
  scroll_area_size_policy.setVerticalStretch(10);
  scroll_area->setSizePolicy(scroll_area_size_policy);
  scroll_area->setWidget(partition_list_frame);
  scroll_area->setWidgetResizable(true);
  scroll_area->setFixedWidth(kWindowWidth);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  bootloader_tip_button_ = new PointerButton(tr(kTextBootloader), this);
  bootloader_tip_button_->setObjectName("bootloader_tip_button");
  bootloader_tip_button_->setFlat(true);
  bootloader_button_ = new PointerButton(this);
  bootloader_button_->setObjectName("bootloader_button");
  bootloader_button_->setFlat(true);

  editing_button_ = new PointerButton(tr(kTextDelete), this);
  editing_button_->setObjectName("editing_button");
  editing_button_->setFlat(true);
  editing_button_->setCheckable(true);
  editing_button_->setChecked(false);

  QHBoxLayout* bottom_layout = new QHBoxLayout();
  bottom_layout->setContentsMargins(15, 10, 15, 10);
  bottom_layout->setSpacing(0);
  // Show bootloader button only if EFI mode is off.
  if (IsEfiEnabled()) {
    bootloader_tip_button_->hide();
    bootloader_button_->hide();
  } else {
    bottom_layout->addWidget(bootloader_tip_button_);
    bottom_layout->addWidget(bootloader_button_);
  }
  bottom_layout->addStretch();
  bottom_layout->addWidget(editing_button_);

  QFrame* bottom_frame = new QFrame();
  bottom_frame->setObjectName("bottom_frame");
  bottom_frame->setContentsMargins(0, 0, 0, 0);
  bottom_frame->setLayout(bottom_layout);
  bottom_frame->setFixedWidth(kWindowWidth - 2);

  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->addWidget(scroll_area, 1, Qt::AlignHCenter);
  main_layout->addSpacing(0);
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
  layout->addSpacing(10);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setFixedWidth(kWindowWidth);
  QSizePolicy container_policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
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
  ClearLayout(partition_layout_);

  for (const Device& device : delegate_->devices()) {
    QLabel* model_label = new QLabel();
    model_label->setObjectName("model_label");
    model_label->setText(GetDeviceModelAndCap(device));
    model_label->setContentsMargins(15, 10, 0, 5);
    partition_layout_->addWidget(model_label, 0, Qt::AlignLeft);
    for (const Partition& partition : device.partitions) {
      if ((partition.type == PartitionType::Extended) || partition.busy) {
        // Ignores extended partition and currently in-used partitions.
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
              this, &AdvancedPartitionFrame::onDeletePartitionTriggered);
    }
  }

  // Add stretch to expand vertically
  partition_layout_->addStretch();
}

void AdvancedPartitionFrame::onDeletePartitionTriggered(
    const Partition& partition) {
  delegate_->deletePartition(partition);
  delegate_->refreshVisual();
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
