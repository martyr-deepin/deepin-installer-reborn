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

// Vertical spacing between error message labels.
const int kMsgLabelSpacing = 2;

}  // namespace

AdvancedPartitionFrame::AdvancedPartitionFrame(PartitionDelegate* delegate_,
                                               QWidget* parent)
    : QFrame(parent),
      delegate_(delegate_),
      error_messages_() {
  this->setObjectName("advanced_partition_frame");

  this->initUI();
  this->initConnections();

  // Hide error message container by default.
  msg_container_frame_->hide();
}

bool AdvancedPartitionFrame::validate() {
  // Clear error messages first.
  this->clearErrorMessages();

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
  const int efi_minimum = GetSettingsInt(kPartitionEFIMinimumSpace);
  Partition root_partition;

  for (const Device& device : delegate_->devices()) {
    for (const Partition& partition : device.partitions) {
      if (partition.mount_point == kMountPointRoot) {
        // Check / partition.
        root_is_set = true;
        root_partition = partition;

      } else if (partition.mount_point == kMountPointBoot) {
        // Check /boot partition.
        boot_is_set = true;
        const qint64 boot_recommend_bytes = boot_recommended * kMebiByte;
        // Add 1Mib to partition size.
        const qint64 boot_real_bytes = partition.getByteLength() + kMebiByte;
        boot_large_enough = (boot_real_bytes >= boot_recommend_bytes);

      } else if (partition.fs == FsType::EFI) {
        // Check EFI partition.
        efi_is_set = true;

        if (partition.status == PartitionStatus::Real) {
          // For existing EFI partition.
          const qint64 efi_minimum_bytes = efi_minimum * kMebiByte;
          const qint64 efi_real_bytes = partition.getByteLength() + kMebiByte;
          efi_large_enough = (efi_real_bytes >= efi_minimum_bytes);
        } else {
          // For newly created EFI partition.
          const qint64 efi_recommended_bytes = efi_recommended * kMebiByte;
          const qint64 efi_real_bytes = partition.getByteLength() + kMebiByte;
          efi_large_enough = (efi_real_bytes >= efi_recommended_bytes);
        }
      } else if (partition.fs == FsType::LinuxSwap) {
        swap_is_set = true;
      }
    }
  }

  if (!root_is_set) {
    addErrorMessage(tr("A root partition is required"),
                    ErrorMessageType::RootMissing);
  }

  const qint64 root_minimum_bytes = root_required * kGibiByte;
  const qint64 root_real_bytes = root_partition.getByteLength() + kMebiByte;
  root_large_enough = (root_real_bytes >= root_minimum_bytes);

  // Check root size only if root is set.
  if (root_is_set && !root_large_enough) {
    addErrorMessage(tr("At least %1 GB is required for root partition")
                        .arg(root_required),
                    ErrorMessageType::RootTooSmall);
  }

  if (IsEfiEnabled()) {
    if (!efi_is_set) {
      addErrorMessage(tr("An EFI partition is required"),
                      ErrorMessageType::EfiMissing);
    }
    if (!efi_large_enough) {
      addErrorMessage(tr("At least %1 MB is required for EFI partition")
                          .arg(efi_recommended),
                      ErrorMessageType::EfiTooSmall);
    }
  }

  if (boot_is_set && !boot_large_enough) {
    addErrorMessage(tr("At least %1 MB is required for /boot partition")
                        .arg( boot_recommended),
                    ErrorMessageType::BootTooSmall);
  }

  if (error_messages_.isEmpty()) {
    // No error found.

    // Create swap file is swap partition is not found.
    WriteRequiringSwapFile(!swap_is_set);
    return true;
  } else {
    return false;
  }
}

void AdvancedPartitionFrame::setBootloaderPath(const QString& bootloader_path) {
  bootloader_button_->setText(bootloader_path);
}

void AdvancedPartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    bootloader_tip_button_->setText(tr("Select location for boot loader"));
    if (editing_button_->isChecked()) {
      editing_button_->setText(tr("Done"));
    } else {
      editing_button_->setText(tr("Delete"));
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
          this, &AdvancedPartitionFrame::clearErrorMessages);
  connect(this, &AdvancedPartitionFrame::requestNewPartitionFrame,
          this, &AdvancedPartitionFrame::clearErrorMessages);
}

void AdvancedPartitionFrame::initUI() {
  msg_head_label_ = new QLabel();
  msg_head_label_->setObjectName("err_msg_top_label");

  msg_layout_ = new QVBoxLayout();
  msg_layout_->setContentsMargins(0, 0, 0, 0);
  msg_layout_->setSpacing(kMsgLabelSpacing);

  QVBoxLayout* msg_container_layout = new QVBoxLayout();
  msg_container_layout->setContentsMargins(0, 0, 0, 0);
  msg_container_layout->setSpacing(0);
  msg_container_layout->addWidget(msg_head_label_);
  msg_container_layout->addSpacing(kMsgLabelSpacing);
  msg_container_layout->addLayout(msg_layout_);

  msg_container_frame_ = new QFrame();
  msg_container_frame_->setObjectName("msg_container_frame");
  msg_container_frame_->setContentsMargins(0, 0, 0, 0);
  msg_container_frame_->setFixedWidth(kWindowWidth);
  msg_container_frame_->setLayout(msg_container_layout);

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

  QVBoxLayout* scroll_layout = new QVBoxLayout();
  scroll_layout->setContentsMargins(0, 0, 0, 0);
  scroll_layout->setSpacing(0);
  scroll_layout->addWidget(msg_container_frame_);
  scroll_layout->addSpacing(8);
  scroll_layout->addWidget(partition_list_frame);

  QFrame* scroll_frame = new QFrame();
  scroll_frame->setObjectName("scroll_frame");
  scroll_frame->setContentsMargins(0, 0, 0, 0);
  scroll_frame->setLayout(scroll_layout);

  QScrollArea* scroll_area = new QScrollArea();
  scroll_area->setObjectName("scroll_area");
  scroll_area->setContentsMargins(0, 0, 0, 0);
  QSizePolicy scroll_area_size_policy(QSizePolicy::Fixed,
                                      QSizePolicy::MinimumExpanding);
  scroll_area_size_policy.setHorizontalStretch(10);
  scroll_area_size_policy.setVerticalStretch(10);
  scroll_area->setSizePolicy(scroll_area_size_policy);
  scroll_area->setWidget(scroll_frame);
  scroll_area->setWidgetResizable(true);
  scroll_area->setFixedWidth(kWindowWidth);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  bootloader_tip_button_ = new PointerButton(
      tr("Select location for boot loader"), this);
  bootloader_tip_button_->setObjectName("bootloader_tip_button");
  bootloader_tip_button_->setFlat(true);
  bootloader_button_ = new PointerButton(this);
  bootloader_button_->setObjectName("bootloader_button");
  bootloader_button_->setFlat(true);

  editing_button_ = new PointerButton(tr("Delete"), this);
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
  bottom_frame->setFixedWidth(kWindowWidth);

  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->addWidget(scroll_area, 1, Qt::AlignHCenter);
  main_layout->addSpacing(2);
  main_layout->addWidget(bottom_frame, 0, Qt::AlignHCenter);
  main_layout->addSpacing(8);

  this->setLayout(main_layout);
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

  const QString installer_device_path(GetInstallerDevicePath());
  for (const Device& device : delegate_->devices()) {
    // Ignores installer device.
    if (installer_device_path.startsWith(device.path)) {
      qDebug() << "Ignore installer device:" << installer_device_path;
      continue;
    }

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
              this, &AdvancedPartitionFrame::onEditPartitionTriggered);
      connect(button, &AdvancedPartitionButton::newPartitionTriggered,
              this, &AdvancedPartitionFrame::onNewPartitionTriggered);
      connect(button, &AdvancedPartitionButton::deletePartitionTriggered,
              this, &AdvancedPartitionFrame::onDeletePartitionTriggered);
    }
  }

  // Add stretch to expand vertically
  partition_layout_->addStretch();
}

void AdvancedPartitionFrame::addErrorMessage(
    const QString& text, AdvancedPartitionFrame::ErrorMessageType type) {
  const ErrorMessage msg = { text, type };
  qDebug() << "error msg:" << text;
  error_messages_.append(msg);

  ClearLayout(msg_layout_);
  for (int i = 0; i < error_messages_.length(); i++) {
    QLabel* error_label = new QLabel();
    if (i == error_messages_.length() - 1) {
      error_label->setObjectName("err_msg_bottom_label");
    } else {
      error_label->setObjectName("err_msg_label");
    }
    error_label->setText(error_messages_.at(i).text);
    msg_layout_->addWidget(error_label);
    error_label->show();
  }

  const int err_count = error_messages_.length();
  // NOTE(xushaohua): Transifex does not ts plural format.
  if (err_count <= 1) {
      msg_head_label_->setText(
          tr("Found %1 error, fix the error to continue install "
             "or switch to simple mode").arg(err_count));
  } else {
    msg_head_label_->setText(
        tr("Found %1 errors, fix the errors to continue install "
           "or switch to simple mode").arg(err_count));
  }

  // Show msg container if it is invisible.
  if (msg_container_frame_->isHidden()) {
    msg_container_frame_->show();
  }
  msg_container_frame_->show();
}

void AdvancedPartitionFrame::clearErrorMessages() {
  error_messages_.clear();
  ClearLayout(msg_layout_);

  // Also hide msg container.
  msg_container_frame_->hide();
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
    editing_button_->setText(tr("Done"));
  } else {
    editing_button_->setText(tr("Delete"));
  }
}

void AdvancedPartitionFrame::onEditPartitionTriggered(
    const Partition& partition) {
  const QString device_path = partition.device_path;
  // Check partition table type first.
  if (! delegate_->isPartitionTableMatch(device_path)) {
    emit this->requestNewTable(device_path);
  } else {
    emit this->requestEditPartitionFrame(partition);
  }
}

void AdvancedPartitionFrame::onNewPartitionTriggered(
    const Partition& partition) {
  const QString device_path = partition.device_path;
  if (! delegate_->isPartitionTableMatch(device_path)) {
    emit this->requestNewTable(device_path);
    return;
  }

  if (delegate_->canAddPrimary(partition, false) ||
      delegate_->canAddLogical(partition, false)) {
    // Switch to NewPartitionFrame only if a new partition can be added.
    emit this->requestNewPartitionFrame(partition);
  } else {
    qWarning() << "Can not add new partition any more" << partition;
    addErrorMessage(tr("No more partition can be created"),
                    ErrorMessageType::NoMorePrimNum);
  }
}

}  // namespace installer