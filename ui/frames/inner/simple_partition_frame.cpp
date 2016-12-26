// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/simple_partition_frame.h"

#include <QButtonGroup>
#include <QDebug>
#include <QEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QShowEvent>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "partman/structs.h"
#include "partman/utils.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/device_model_label.h"
#include "ui/widgets/simple_partition_button.h"
#include "ui/utils/widget_util.h"

namespace installer {

namespace {

// 4 partitions are displays at each row.
const int kPartitionColumns = 4;

const int kWindowWidth = 960;

const char kTextTip[] = "Install here";

FsType GetPartitionDefaultFs() {
  return GetFsTypeByName(GetSettingsString(kPartitionDefaultFs));
}

}  // namespace

SimplePartitionFrame::SimplePartitionFrame(PartitionDelegate* delegate,
                                           QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName("simple_partition_frame");

  this->initUI();
  this->initConnections();
  qRegisterMetaType<Partition>("Partition");
}

bool SimplePartitionFrame::validate() {
  bool efi_is_set = false;
  bool efi_large_enough = false;
  bool root_is_set = false;
  bool root_large_enough = false;
  bool swap_is_set = false;
  bool boot_is_set = false;
  bool boot_large_enough = false;

  const int root_required = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
  const int root_with_swap_required =
      GetSettingsInt(kPartitionMinimumDiskSpaceRequiredInLowMemory);
  const int boot_recommended = GetSettingsInt(kPartitionDefaultBootSpace);
  const int efi_recommended = GetSettingsInt(kPartitionDefaultEFISpace);

  Partition root_partition;

  // First check whether partitions are modified in advanced mode.
  for (const Device& device : delegate_->devices()) {
    for (const Partition& partition : device.partitions) {
      if (partition.mount_point == kMountPointRoot) {
        // Check / partition.
        root_is_set = true;
        root_partition = partition;

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

  bool is_max_prims_reached = false;
  if (!root_is_set) {
    SimplePartitionButton* button =
        qobject_cast<SimplePartitionButton*>(button_group_->checkedButton());
    if (button) {
      root_is_set = true;
      root_partition = button->partition();
    } else {
      root_is_set = false;
    }
  }

  // If currently selected device reaches its maximum primary partitions and
  // button->partition() is a Freespace, it is impossible to created a new
  // primary partition.
  if ((root_partition.type == PartitionType::Unallocated) &&
      !delegate_->canAddPrimary(root_partition) &&
      !delegate_->canAddLogical(root_partition)) {
    is_max_prims_reached = true;
  }

  if (!root_is_set) {
    msg_label_->setText(tr("Please select one partition"));
    return false;
  }

  if (is_max_prims_reached) {
    msg_label_->setText(tr("No more partition can be created!"));
    return false;
  }

  if (swap_is_set) {
    const qint64 minimum_bytes = root_required * kGibiByte;
    root_large_enough = (root_partition.getByteLength() > minimum_bytes);
  } else {
    // If no swap partition is created, more space is required for
    // root partition.
    const qint64 minimum_bytes = root_with_swap_required * kGibiByte;
    root_large_enough = (root_partition.getByteLength() > minimum_bytes);
  }

  if (!root_large_enough) {
    msg_label_->setText(
        tr("At least %1 Gib is required for root partition.")
            .arg(root_required));
    return false;
  }

  if (IsEfiEnabled() && efi_is_set && !efi_large_enough) {
    msg_label_->setText(
        tr("At least %1 Mib is required for EFI partition.")
            .arg(efi_recommended));
    return false;
  }

  if (boot_is_set && !boot_large_enough) {
    msg_label_->setText(
        tr("At least %1 Mib is required for /boot partition.")
            .arg(boot_recommended));
    return false;
  }

  // Create swap file is swap partition is not found.
  WriteRequiringSwapFile(!swap_is_set);

  msg_label_->clear();
  return true;
}

void SimplePartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    tip_label_->setText(tr(kTextTip));
  } else {
    QFrame::changeEvent(event);
  }
}

void SimplePartitionFrame::showEvent(QShowEvent* event) {
  QFrame::showEvent(event);

  QAbstractButton* button = button_group_->checkedButton();
  if (button) {
    // Display install_tip explicitly.
    this->showInstallTip(button);
  }
}


void SimplePartitionFrame::appendOperations() {
  // Reset simple operations first.
  delegate_->resetSimpleOperations();

  bool efi_is_set = false;
  for (const Device& device : delegate_->devices()) {
    for (const Partition& partition : device.partitions) {
      if (partition.fs == FsType::EFI) {
        efi_is_set = true;
      }
    }
  }

  SimplePartitionButton* button =
      qobject_cast<SimplePartitionButton*>(button_group_->checkedButton());
  Q_ASSERT(button);
  Partition partition = button->partition();
  if (IsEfiEnabled() && !efi_is_set) {
    if (partition.type == PartitionType::Normal) {
      // Delete partition first.
      partition = delegate_->deleteSimplePartition(partition);
    }
    // Create root partition and EFI partition.
    const int efi_space = GetSettingsInt(kPartitionDefaultEFISpace);
    const qint64 efi_sectors = efi_space * kMebiByte / partition.sector_size;
    // Create EFI partition from start.
    delegate_->createPartition(partition,
                               PartitionType::Normal,
                               true,
                               FsType::EFI,
                               "",
                               efi_sectors);

    // Create root partition from end.
    const qint64 remaining_sectors = partition.getSectorLength() - efi_sectors;
    delegate_->createPartition(partition,
                               PartitionType::Normal,
                               false,
                               GetPartitionDefaultFs(),
                               kMountPointRoot,
                               remaining_sectors);
  } else {
    // Only create root partition.
    if (partition.type == PartitionType::Unallocated) {
      // First try to add logical partition, then primary partition.
      if (delegate_->canAddLogical(partition)) {
        delegate_->createSimplePartition(partition,
                                         PartitionType::Logical,
                                         false,
                                         GetPartitionDefaultFs(),
                                         kMountPointRoot,
                                         partition.getSectorLength());
      } else if (delegate_->canAddPrimary(partition)) {
        delegate_->createSimplePartition(partition,
                                         PartitionType::Normal,
                                         false,
                                         GetPartitionDefaultFs(),
                                         kMountPointRoot,
                                         partition.getSectorLength());
      } else {
        // We shall never reach here.
        qCritical() << "Reached maximum primary partitions:" << partition.path;
      }
    } else {
      delegate_->formatSimplePartition(button->partition(),
                                       GetPartitionDefaultFs(),
                                       kMountPointRoot);
    }
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

  QScrollArea* scroll_area = new QScrollArea();
  scroll_area->setWidget(grid_wrapper);
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
  for (const Device& device : delegate_->devices()) {
    DeviceModelLabel* device_model_label = new DeviceModelLabel();
    device_model_label->setText(GetDeviceModelAndCap(device));
    device_model_label->setFixedSize(kWindowWidth, 20);

    // Make sure that widgets in grid are left-aligned.
    grid_layout_->addWidget(device_model_label, row, 0,
                            1, kPartitionColumns, Qt::AlignLeft);
    row += 1;

    for (const Partition& partition : device.partitions) {
      if ((partition.type == PartitionType::Extended) || partition.busy) {
        // Ignores extended partition or currently in-used partitions.
        continue;
      }
      SimplePartitionButton* button = new SimplePartitionButton(partition);
      button_group_->addButton(button);
      grid_layout_->addWidget(button, row, column, Qt::AlignHCenter);
      // Select root partition.
      if (partition.mount_point == kMountPointRoot) {
        // Do not clear mount point by blocking emitting signals.
        button_group_->blockSignals(true);
        button->setChecked(true);
        button_group_->blockSignals(false);
        this->showInstallTip(button);
      }

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

void SimplePartitionFrame::showInstallTip(QAbstractButton* button) {
  // Move install_tip to bottom of button
  const QPoint pos = button->pos();
  install_tip_->move(pos.x(), pos.y() - 10);
  install_tip_->show();
}

void SimplePartitionFrame::onDeviceRefreshed() {
  this->repaintDevices();
}

void SimplePartitionFrame::onPartitionButtonToggled(QAbstractButton* button,
                                                    bool checked) {
  if (checked) {
    this->showInstallTip(button);

    // Clear warning message.
    msg_label_->clear();
  }

  SimplePartitionButton* part_button =
      qobject_cast<SimplePartitionButton*>(button);
  if (part_button) {
    if (!checked) {
      // Clear mount point of old root partition.
      delegate_->updateMountPoint(part_button->partition(), "");

      // Do not refresh partition list here. Instead, call
      // delegate_->refreshVisual() in PartitionFrame before switching to
      // AdvancedPartitionFrame.
    } else if(part_button->partition().mount_point != kMountPointRoot) {
      // Clear mount point of new partition if it is not root.
      delegate_->updateMountPoint(part_button->partition(), "");

      if (this->validate()) {
        this->appendOperations();
      }
    }
  }
}

}  // namespace installer