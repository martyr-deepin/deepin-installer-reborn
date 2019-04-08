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

#include "ui/frames/inner/edit_partition_frame.h"

#include <QCheckBox>
#include <QEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/delegates/advanced_partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/models/fs_model.h"
#include "ui/models/mount_point_model.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/rounded_progress_bar.h"
#include "ui/widgets/table_combo_box.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const int kWindowWidth = 640;
const int kProgressBarWidth = 280;

const int kContentSpacing = 15;

// Check whether partition with |mount_point| should be formatted
// compulsively.
bool IsInFormattedMountPointList(const QString& mount_point) {
  const QStringList list(GetSettingsStringList(kPartitionFormattedMountPoints));
  return list.contains(mount_point);
}

}  // namespace

EditPartitionFrame::EditPartitionFrame(AdvancedPartitionDelegate* delegate,
                                       QWidget* parent)
    : QFrame(parent),
      delegate_(delegate),
      partition_() {
  this->setObjectName("edit_partition_frame");

  this->initUI();
  this->initConnections();
}

void EditPartitionFrame::setPartition(const Partition::Ptr partition) {
  // Update partition information.
  partition_ = partition;
  os_label_->setPixmap(QPixmap(GetOsTypeLargeIcon(partition->os)));
  name_label_->setText(GetPartitionLabelAndPath(partition));
  usage_label_->setText(GetPartitionUsage(partition));
  usage_bar_->setValue(GetPartitionUsageValue(partition));

  // Update fs list.
  fs_model_->setShowEFI(!delegate_->isMBRPreferred());
  fs_model_->setShowUnknown(partition->fs == FsType::Unknown);

  // Reset fs index.
  int fs_index = fs_model_->index(partition->fs);
  if (fs_index == -1) {
    // Get index of unused filesystem in filesystem list.
    fs_index = fs_model_->index(FsType::Empty);
  }
  // If partition fs type not in current fs_list, select nothing.
  fs_box_->setCurrentIndex(fs_index);

  // Reset mount point box. mount_point might be empty.
  const int mp_index = mount_point_model_->index(partition->mount_point);
  mount_point_box_->setCurrentIndex(mp_index);

  updateFormatBoxState();
}

void EditPartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Edit Disk"));
    comment_label_->setText(
        tr("Make sure you have backed up important data, then select the disk to install"));
    fs_label_->setText(tr("Filesystem"));
    mount_point_label_->setText(tr("Mount point"));
    format_label_->setText(tr("Format the partition"));
    cancel_button_->setText(tr("Cancel"));
    ok_button_->setText(tr("OK"));
  } else {
    QFrame::changeEvent(event);
  }
}

void EditPartitionFrame::forceFormat(bool force) {
  format_check_box_->setChecked(force);
  format_check_box_->setEnabled(!force);
}

void EditPartitionFrame::updateFormatBoxState() {
  const FsType fs_type = fs_model_->getFs(fs_box_->currentIndex());
  const Partition::Ptr real_partition(delegate_->getRealPartition(partition_));
  const int mp_index = mount_point_box_->currentIndex();
  QString mount_point;

  // Read mount point only if current filesystem supports mount-point.
  if (IsMountPointSupported(fs_type)) {
    mount_point = mount_point_model_->getMountPoint(mp_index);
  }

  // If current partition type is PartitionType::Unallocated,
  // real_partition is nullptr, need format.
  // But if status is PartitionStatus::New, will always format
  bool format = (real_partition.isNull() || partition_->status == PartitionStatus::New);


  // NOTE: maybe need check partition->path.isEmpty()
  if (!format) {
      // Format this partition compulsively if its fs type changed.
      format = (fs_type != real_partition->fs);
  }

  // Format this partition if mount-point is in formatted-mount-point list.
  format |= IsInFormattedMountPointList(mount_point);
  this->forceFormat(format);

  // If it is linux-swap, empty or known, hide format box.
  bool invisible = (fs_type == FsType::LinuxSwap ||
      fs_type == FsType::Empty ||
      fs_type == FsType::Unknown);
  format_label_->setVisible(!invisible);
  format_check_box_->setVisible(!invisible);
}

void EditPartitionFrame::initConnections() {
  connect(fs_box_,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &EditPartitionFrame::onFsChanged);
  connect(mount_point_box_,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &EditPartitionFrame::onMountPointChanged);

  // Does nothing when cancel-button is clicked.
  connect(cancel_button_, &QPushButton::clicked,
          this, &EditPartitionFrame::finished);
  connect(ok_button_, &QPushButton::clicked,
          this, &EditPartitionFrame::onOkButtonClicked);
}

void EditPartitionFrame::initUI() {
  title_label_ = new TitleLabel(tr("Edit Disk"));
  comment_label_ = new CommentLabel(
      tr("Make sure you have backed up important data, then select the disk to install"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  os_label_ = new QLabel();
  os_label_->setObjectName("os_label");
  name_label_ = new QLabel();
  name_label_->setObjectName("name_label");
  usage_label_ = new QLabel();
  usage_label_->setObjectName("usage_label");

  QHBoxLayout* name_layout = new QHBoxLayout();
  name_layout->setContentsMargins(0, 0, 0, 0);
  name_layout->setSpacing(0);
  name_layout->addWidget(name_label_);
  name_layout->addStretch();
  name_layout->addWidget(usage_label_);
  QFrame* name_frame = new QFrame();
  name_frame->setObjectName("name_frame");
  name_frame->setContentsMargins(0, 0, 0, 0);
  name_frame->setLayout(name_layout);
  name_frame->setFixedWidth(kProgressBarWidth);

  usage_bar_ = new RoundedProgressBar();
  usage_bar_->setFixedSize(kProgressBarWidth, 8);

  QLabel* separator_label = new QLabel();
  separator_label->setObjectName("separator_label");
  separator_label->setFixedSize(560, 2);

  fs_label_ = new QLabel(tr("Filesystem"));
  fs_label_->setObjectName("fs_label");
  mount_point_label_ = new QLabel(tr("Mount point"));
  mount_point_label_->setObjectName("mount_point_label");
  format_label_ = new QLabel(tr("Format the partition"));
  format_label_->setObjectName("format_label");

  fs_box_ = new TableComboBox();
  fs_box_->setObjectName("fs_box");
  fs_model_ = new FsModel(delegate_->getFsTypeList(), this);
  fs_box_->setModel(fs_model_);

  mount_point_box_ = new TableComboBox();
  mount_point_box_->setObjectName("mount_point_box");
  mount_point_model_ = new MountPointModel(delegate_->getMountPoints(), this);
  mount_point_box_->setModel(mount_point_model_);

  format_check_box_ = new QCheckBox();
  format_check_box_->setObjectName("format_check_box");
  format_check_box_->setFixedWidth(20);

  QHBoxLayout* format_layout =new QHBoxLayout();
  format_layout->setContentsMargins(0, 0, 0, 0);
  format_layout->setSpacing(0);
  format_layout->addWidget(format_check_box_);
  format_layout->addSpacing(10);
  format_layout->addWidget(format_label_);
  format_layout->addStretch();

  QVBoxLayout* content_layout = new QVBoxLayout();
  content_layout->setContentsMargins(0, 0, 0, 0);
  content_layout->setSpacing(3);
  content_layout->addWidget(fs_label_);
  content_layout->addWidget(fs_box_);
  content_layout->addSpacing(kContentSpacing);
  content_layout->addWidget(mount_point_label_);
  content_layout->addWidget(mount_point_box_);
  content_layout->addSpacing(kContentSpacing);
  content_layout->addLayout(format_layout);

  QFrame* content_frame = new QFrame();
  content_frame->setObjectName("content_frame");
  content_frame->setContentsMargins(0, 0, 0, 0);
  content_frame->setLayout(content_layout);
  // Same width as with table combobox.
  content_frame->setFixedWidth(mount_point_box_->width());

  cancel_button_ = new NavButton(tr("Cancel"));
  ok_button_ = new NavButton(tr("OK"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(os_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(10);
  layout->addWidget(name_frame, 0, Qt::AlignHCenter);
  layout->addSpacing(10);
  layout->addWidget(usage_bar_, 0, Qt::AlignHCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addWidget(separator_label, 0, Qt::AlignHCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addWidget(content_frame, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(cancel_button_, 0, Qt::AlignHCenter);
  layout->addSpacing(30);
  layout->addWidget(ok_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setStyleSheet(ReadFile(":/styles/edit_partition_frame.css"));
}

void EditPartitionFrame::onFsChanged(int index) {
  // Hide mount point box if current selected filesystem does not need
  // mount point.
  const FsType fs_type = fs_model_->getFs(index);
  const bool visible = IsMountPointSupported(fs_type);
  mount_point_label_->setVisible(visible);
  mount_point_box_->setVisible(visible);

  this->updateFormatBoxState();
}

void EditPartitionFrame::onMountPointChanged(int index) {
  const QString mount_point = mount_point_model_->getMountPoint(index);
  const FsType fs_type = fs_model_->getFs(fs_box_->currentIndex());
  const FsType default_fs = GetDefaultFsType();
  const int default_fs_index = fs_model_->index(default_fs);

  // If mount point is not empty, partition fs shall be not empty and can not
  // be linux-swap or efi.
  if (!mount_point.isEmpty()) {
    // If fs is empty, select default fs.
    if (fs_type == FsType::Empty ||
        fs_type == FsType::LinuxSwap ||
        fs_type == FsType::EFI) {
      fs_box_->setCurrentIndex(default_fs_index);
    }
  }

  this->updateFormatBoxState();
}

void EditPartitionFrame::onOkButtonClicked() {
  const FsType fs_type = fs_model_->getFs(fs_box_->currentIndex());
  QString mount_point;

  // Read mount point only if current filesystem supports mount-point.
  if (IsMountPointSupported(fs_type)) {
    const int index = mount_point_box_->currentIndex();
    mount_point = mount_point_model_->getMountPoint(index);
  }

  if (format_check_box_->isChecked()) {
    // Create an OperationFormat object.
    delegate_->formatPartition(partition_, fs_type, mount_point);
  } else {
    // If FormatOperation is reverted.
    if (partition_->status == PartitionStatus::Format) {
      delegate_->unFormatPartition(partition_);
      // Reset partition status.
      partition_->status = PartitionStatus::Real;
    }

    // Create an OperationMountPoint object.
    if (partition_->mount_point != mount_point) {
      delegate_->updateMountPoint(partition_, mount_point);
    }
  }
  delegate_->refreshVisual();

  emit this->finished();
}

}  // namespace installer
