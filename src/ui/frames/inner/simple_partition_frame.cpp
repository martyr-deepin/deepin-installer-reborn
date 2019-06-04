/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#include "ui/frames/inner/simple_partition_frame.h"

#include <QButtonGroup>
#include <QDebug>
#include <QEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QShowEvent>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/simple_partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/device_model_label.h"
#include "ui/widgets/simple_partition_button.h"
#include "ui/utils/widget_util.h"

namespace installer {

namespace {

// 4 partitions are displays at each row.
const int kDiskColumns = 4;

const int kWindowWidth = 960;

}  // namespace

SimplePartitionFrame::SimplePartitionFrame(SimplePartitionDelegate* delegate,
                                           QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName("simple_partition_frame");

  this->initUI();
  this->initConnections();
  qRegisterMetaType<Partition>("Partition");
}

bool SimplePartitionFrame::validate() {
  switch (delegate_->validate()) {
    case SimpleValidateState::Ok: {
        qDebug() << "all ok!";
        msg_label_->clear();
        return true;
    }
    case SimpleValidateState::MaxPrimPartErr: {
        qDebug() << "partition size error!";
        msg_label_->setText(
            tr("Unable to create new partition, please "
               "select one of the existing partitions!"));
        break;
    }
    case SimpleValidateState::RootMissing: {
        qDebug() << "not have root partition!";
        msg_label_->setText(
            tr("Please select one of the partitions to install!"));
        break;
    }
    case SimpleValidateState::RootTooSmall: {
        qDebug() << "root partition is too small!";
        const int root_required =
            GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
        msg_label_->setText(tr("At least %1 GB is required for root partition")
                                .arg(root_required));
        break;
    }
  }

  return false;
}

Device::Ptr SimplePartitionFrame::selectedDevice() const
{
    Partition::Ptr partition = delegate_->selectedPartition();

    Q_ASSERT(partition);

    for (Device::Ptr device : delegate_->real_devices()) {
        if (device->path == partition->device_path) {
            return device;
        }
    }

    Q_ASSERT(false);

    return Device::Ptr();
}

void SimplePartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    tip_label_->setText(tr("Install here"));
  } else {
    QFrame::changeEvent(event);
  }
}

void SimplePartitionFrame::showEvent(QShowEvent* event) {
  QFrame::showEvent(event);

  QAbstractButton* button = button_group_->checkedButton();
  if (button) {
    SimplePartitionButton* part_button =
        dynamic_cast<SimplePartitionButton*>(button);
    Q_ASSERT(part_button);
    if (part_button && part_button->selected()) {
      // Display install_tip is button is selected.
      this->showInstallTip(button);
    }
  }
}

void SimplePartitionFrame::appendOperations() {
  // Reset simple operations.
  delegate_->resetOperations();

  bool found_efi = false;
  for (const Device::Ptr device : delegate_->virtual_devices()) {
    for (const Partition::Ptr partition : device->partitions) {
      if (partition->fs == FsType::EFI) {
        found_efi = true;
      }
    }
  }

  SimplePartitionButton* button =
      dynamic_cast<SimplePartitionButton*>(button_group_->checkedButton());
  Q_ASSERT(button);
  if (!button) {
    qCritical() << "no partition button selected";
    return;
  }

  Partition::Ptr partition = button->partition();
  if (!delegate_->isMBRPreferred() && !found_efi) {
    if (partition->type == PartitionType::Normal) {
      // Delete normal partition first.
      partition = delegate_->deletePartition(partition);
    }

    // Create root partition and EFI partition
    const int efi_space = GetSettingsInt(kPartitionDefaultEFISpace);
    const qint64 efi_sectors = efi_space * kMebiByte / partition->sector_size;
    // Create EFI partition from start.
    delegate_->createPartition(partition,
                               PartitionType::Normal,
                               true,
                               FsType::EFI,
                               "",
                               efi_sectors);

    // Create root partition from end.
    const qint64 remaining_sectors = partition->getSectorLength() - efi_sectors;
    delegate_->createPartition(partition,
                               PartitionType::Normal,
                               false,
                               GetDefaultFsType(),
                               kMountPointRoot,
                               remaining_sectors);
  } else {
    // Only create root partition
    if (partition->type == PartitionType::Unallocated) {
      // First try to add primary partition, then logical partition->
      if (delegate_->canAddPrimary(partition)) {
        delegate_->createPartition(partition,
                                   PartitionType::Normal,
                                   false,
                                   GetDefaultFsType(),
                                   kMountPointRoot,
                                   partition->getSectorLength());
      } else if (delegate_->canAddLogical(partition)) {
        delegate_->createPartition(partition,
                                   PartitionType::Logical,
                                   false,
                                   GetDefaultFsType(),
                                   kMountPointRoot,
                                   partition->getSectorLength());
      }  else {
        // We shall never reach here.
        qCritical() << "Reached maximum primary partitions:" << partition->path;
      }
    } else {
      // Format real partition->
      delegate_->formatPartition(button->partition(),
                                 GetDefaultFsType(),
                                 kMountPointRoot);
    }
  }

  // Update simple bootloader path.
  const QString bootloader_path = button->partition()->device_path;
  delegate_->setBootloaderPath(bootloader_path);
}

void SimplePartitionFrame::initConnections() {
  // Repaint layout only when real device list is updated.
  connect(delegate_, &SimplePartitionDelegate::deviceRefreshed,
          this, &SimplePartitionFrame::onDeviceRefreshed);
  connect(button_group_,
          static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>
          (&QButtonGroup::buttonToggled),
          this, &SimplePartitionFrame::onPartitionButtonToggled);
}

void SimplePartitionFrame::initUI() {
  button_group_ = new QButtonGroup(this);

  QLabel* tip_icon = new QLabel();
  tip_icon->setPixmap(installer::renderPixmap(":/images/install_icon.svg"));

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

  msg_label_ = new QLabel();
  msg_label_->setObjectName("msg_label");
  msg_label_->setFixedHeight(20);

  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->addWidget(scroll_area, Qt::AlignHCenter);
  main_layout->addStretch();
  main_layout->addWidget(msg_label_, 0, Qt::AlignHCenter);
  main_layout->addSpacing(10);

  this->setLayout(main_layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setFixedWidth(kWindowWidth);
  QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  policy.setVerticalStretch(100);
  this->setSizePolicy(policy);
  this->setStyleSheet(ReadFile(":/styles/simple_partition_frame.css"));
}

void SimplePartitionFrame::repaintDevices() {
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
  for (const Device::Ptr device : delegate_->virtual_devices()) {
    DeviceModelLabel* device_model_label = new DeviceModelLabel();
    device_model_label->setText(GetDeviceModelCapAndPath(device));
    device_model_label->setFixedSize(kWindowWidth, 20);

    // Make sure that widgets in grid are left-aligned.
    grid_layout_->addWidget(device_model_label, row, 0,
                            1, kDiskColumns, Qt::AlignLeft);
    row += 1;

    for (const Partition::Ptr partition : device->partitions) {
      if ((partition->type == PartitionType::Extended) || partition->busy) {
        // Ignores extended partition or currently in-used partitions.
        continue;
      }

      // Filter unallocated partitions which are larger than 2MiB.
      if (partition->type == PartitionType::Unallocated &&
          partition->getByteLength() <= 2 * kMebiByte) {
        continue;
      }
      SimplePartitionButton* button = new SimplePartitionButton(partition);
      button_group_->addButton(button);
      grid_layout_->addWidget(button, row, column, Qt::AlignHCenter);
      // Select root partition->
      if (partition->mount_point == kMountPointRoot) {
        // Do not clear mount point by blocking emitting signals.
        button_group_->blockSignals(true);
        button->setChecked(true);
        button_group_->blockSignals(false);
        this->showInstallTip(button);
      }
      connect(button, &QPushButton::clicked,
              this, &SimplePartitionFrame::onPartitionButtonClicked);

      column += 1;
      // Add rows.
      if (column >= kDiskColumns) {
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
  QLabel* place_holder_label = new QLabel(this);
  place_holder_label->setObjectName("place_holder_label");
  place_holder_label->setFixedSize(kWindowWidth, 30);
  grid_layout_->addWidget(place_holder_label, row, 0,
                          1, kDiskColumns, Qt::AlignHCenter);

  // Resize grid_wrapper explicitly.
  grid_wrapper_->adjustSize();
}

void SimplePartitionFrame::showInstallTip(QAbstractButton* button) {
  // Move install_tip to bottom of button
  const QPoint pos = button->pos();
  install_tip_->move(pos.x(), pos.y() - 10);
  install_tip_->show();
}

void SimplePartitionFrame::onDeviceRefreshed() {
  this->repaintDevices();
}

void SimplePartitionFrame::onPartitionButtonClicked() {
  // Clear warning message first.
  msg_label_->clear();

  // Hide tooltip.
  install_tip_->hide();

  SimplePartitionButton* button =
      dynamic_cast<SimplePartitionButton*>(this->sender());
  Q_ASSERT(button);
  if (!button) {
    qCritical() << "no partition button is selected";
    return;
  }

  // Check partition table type.
  const QString device_path = button->partition()->device_path;
  if (!delegate_->isPartitionTableMatch(device_path)) {
    emit this->requestNewTable(device_path);
    return;
  }

  // Update selected partition->
  delegate_->selectPartition(button->partition());

  if (this->validate()) {
    button->setSelected(true);

    // Show install-tip at bottom of current checked button.
    this->showInstallTip(button);

    // If selected partitions contains windows system, show another message.
    if (button->partition()->os == OsType::Windows) {
      msg_label_->setText(tr("Windows will not boot if install deepin on "
                             "Windows disk, please confirm to continue"));
    }

    this->appendOperations();
  }
}

void SimplePartitionFrame::onPartitionButtonToggled(QAbstractButton* button,
                                                    bool checked) {
  SimplePartitionButton* part_button =
      dynamic_cast<SimplePartitionButton*>(button);
  if (!part_button) {
    qCritical() << "no partition button is selected";
    return;
  }

  if (!checked) {
    // Deselect previous button.
    part_button->setSelected(false);
    return;
  }
}

}  // namespace installer
