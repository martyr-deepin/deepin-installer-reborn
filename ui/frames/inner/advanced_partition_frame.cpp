// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/advanced_partition_frame.h"

#include <QButtonGroup>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/advanced_partition_button.h"
#include "ui/utils/widget_util.h"

namespace installer {

namespace {

const int kWindowWidth = 960;

// Vertical spacing between error message labels.
const int kMsgLabelSpacing = 2;

}  // namespace

AdvancedPartitionFrame::AdvancedPartitionFrame(
    AdvancedPartitionDelegate* delegate_, QWidget* parent)
    : QFrame(parent),
      delegate_(delegate_),
      validate_states_() {
  this->setObjectName("advanced_partition_frame");

  this->initUI();
  this->initConnections();

  // Hide error message container by default.
  msg_container_frame_->hide();
}

bool AdvancedPartitionFrame::validate() {
  // Clear error messages first.
  this->clearErrorMessages();

  validate_states_ = delegate_->validate();
  if (validate_states_.isEmpty()) {
    return true;
  } else {
    this->showErrorMessages();

    // Also, scroll main content area to top.
    scroll_area_->verticalScrollBar()->setValue(0);

    return false;
  }
}

void AdvancedPartitionFrame::setBootloaderPath(const QString& bootloader_path) {
  bootloader_button_->setText(bootloader_path);
}

void AdvancedPartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    bootloader_tip_button_->setText(tr("Change boot loader"));
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
  connect(delegate_, &AdvancedPartitionDelegate::deviceRefreshed,
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

  scroll_area_ = new QScrollArea();
  scroll_area_->setObjectName("scroll_area");
  scroll_area_->setContentsMargins(0, 0, 0, 0);
  QSizePolicy scroll_area_size_policy(QSizePolicy::Fixed,
                                      QSizePolicy::MinimumExpanding);
  scroll_area_size_policy.setHorizontalStretch(10);
  scroll_area_size_policy.setVerticalStretch(10);
  scroll_area_->setSizePolicy(scroll_area_size_policy);
  scroll_area_->setWidget(scroll_frame);
  scroll_area_->setWidgetResizable(true);
  scroll_area_->setFixedWidth(kWindowWidth);
  scroll_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  bootloader_tip_button_ = new PointerButton(tr("Change boot loader"), this);
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
  if (!delegate_->isMBRPreferred()) {
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
  main_layout->addWidget(scroll_area_, 1, Qt::AlignHCenter);
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

  for (const Device& device : delegate_->virtual_devices()) {
    QLabel* model_label = new QLabel();
    model_label->setObjectName("model_label");
    model_label->setText(GetDeviceModelCapAndPath(device));
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

void AdvancedPartitionFrame::showErrorMessages() {
  ClearLayout(msg_layout_);

  for (int i = 0; i < validate_states_.length(); i++) {
    QLabel* error_label = new QLabel();
    if (i == validate_states_.length() - 1) {
      error_label->setObjectName("err_msg_bottom_label");
    } else {
      error_label->setObjectName("err_msg_label");
    }

    const QString text = this->validateStateToText(validate_states_.at(i));
    error_label->setText(text);
    msg_layout_->addWidget(error_label);
    error_label->show();
  }

  const int err_count = validate_states_.length();
  // NOTE(xushaohua): Transifex does not ts plural format.
  if (err_count <= 1) {
    msg_head_label_->setText(
        tr("%1 error found, fix to continue installation or "
           "switch to simple mode").arg(err_count));
  } else {
    msg_head_label_->setText(
        tr("%1 errors found, fix to continue installation or "
           "switch to simple mode").arg(err_count));
  }

  // Show msg container if it is invisible.
  if (msg_container_frame_->isHidden()) {
    msg_container_frame_->show();
  }
}

QString AdvancedPartitionFrame::validateStateToText(AdvancedValidateState state) {
  switch (state) {
    case AdvancedValidateState::BootFsInvalid: {
      const FsTypeList boot_fs_list = delegate_->getBootFsTypeList();
      QStringList fs_name_list;
      for (const FsType& fs_type : boot_fs_list) {
        fs_name_list.append(GetFsTypeName(fs_type));
      }
      const QString fs_name(fs_name_list.join("/"));
      return tr("The partition filesystem type of /boot directory "
                "can only be %1 ").arg(fs_name);
    }
    case AdvancedValidateState::BootPartNumberInvalid: {
      return tr("The partition of /boot directory should be "
                "the first partition on hard disk");
    }
    case AdvancedValidateState::BootTooSmall: {
      const int boot_recommended = GetSettingsInt(kPartitionDefaultBootSpace);
      return tr("At least %1 MB is required for /boot partition")
          .arg( boot_recommended);
    }
    case AdvancedValidateState::EfiMissing: {
      return tr("Add an EFI partition to continue");
    }
    case AdvancedValidateState::EfiTooSmall: {
      const int efi_recommended = GetSettingsInt(kPartitionDefaultEFISpace);
      return tr("At least %1 MB is required for EFI partition")
          .arg(efi_recommended);
    }
    case AdvancedValidateState::RootMissing: {
      return tr("Add a Root partition to continue");
    }
    case AdvancedValidateState::RootTooSmall: {
      const int root_required =
          GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
      return tr("At least %1 GB is required for Root partition")
          .arg(root_required);
    }
    default: {
      // We shall never reach here.
      return "";
    }
  }
}

void AdvancedPartitionFrame::clearErrorMessages() {
  validate_states_.clear();
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

  if (delegate_->canAddPrimary(partition) ||
      delegate_->canAddLogical(partition)) {
    // Switch to NewPartitionFrame only if a new partition can be added.
    emit this->requestNewPartitionFrame(partition);
  } else {
    qWarning() << "Can not add new partition any more" << partition;
    emit this->requestPartitionNumberLimitationFrame();
  }
}

}  // namespace installer