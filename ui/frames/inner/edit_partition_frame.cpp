// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/edit_partition_frame.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QtCore/QEvent>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/models/fs_model.h"
#include "ui/models/mount_point_model.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/table_combo_box.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const int kWindowWidth = 640;
const int kProgressBarWidth = 280;

const char kTextTitle[] = "Edit Disk";
const char kTextComment[] = "Please make sure you have backed up "
    "important data, then select the disk  to install";
const char kTextFilesystem[] = "Filesystem";
const char kTextMountPoint[] = "Mount point";
const char kTextFormat[] = "Format the partition";
const char kTextCancel[] = "Cancel";
const char kTextOk[] = "OK";

}  // namespace

EditPartitionFrame::EditPartitionFrame(PartitionDelegate* delegate,
                                       QWidget* parent)
    : QFrame(parent),
      delegate_(delegate),
      partition_() {
  this->setObjectName("edit_partition_frame");

  this->initUI();
  this->initConnections();
}

void EditPartitionFrame::setPartition(const Partition& partition) {
  partition_ = partition;

  os_label_->setPixmap(QPixmap(GetOsTypeLargeIcon(partition.os)));
  name_label_->setText(GetPartitionLabelAndPath(partition));
  usage_label_->setText(GetPartitionUsage(partition));
  usage_bar_->setValue(GetPartitionUsageValue(partition));

  // Reset fs index.
  int fs_index = fs_model_->index(partition.fs);
  if (fs_index == -1) {
    fs_index = 0;
  }
  fs_box_->setCurrentIndex(fs_index);

  // Reset mount point box.
  int mount_point_index = mount_point_model_->index(partition.mount_point);
  if (mount_point_index == -1) {
    mount_point_index = 0;
  }
  mount_point_box_->setCurrentIndex(mount_point_index);

  switch (partition_.status) {
    case PartitionStatus::Format: {
      // Compare between real filesystem type and current one.
      const Partition real_partition(delegate_->getRealPartition(partition_));
      format_check_box_->setEnabled(real_partition.fs == partition_.fs);
      format_check_box_->setChecked(true);
      break;
    }
    case PartitionStatus::New: {
      // Force format.
      this->forceFormat(true);
      break;
    }
    case PartitionStatus::Real: {
      this->forceFormat(false);
      break;
    }
    default: {
      break;
    }
  }
}

void EditPartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
    fs_label_->setText(tr(kTextFilesystem));
    mount_point_label_->setText(tr(kTextMountPoint));
    format_label_->setText(tr(kTextFormat));
    cancel_button_->setText(tr(kTextCancel));
    ok_button_->setText(tr(kTextOk));
  } else {
    QFrame::changeEvent(event);
  }
}

void EditPartitionFrame::forceFormat(bool force) {
  format_check_box_->setChecked(force);
  format_check_box_->setEnabled(!force);
}

void EditPartitionFrame::updateFormatBoxState() {
  // Format partition forcefully if its type is changed.
  const FsType fs_type = fs_model_->getFs(fs_box_->currentIndex());
  const Partition real_partition(delegate_->getRealPartition(partition_));
  const QString mount_point =
      mount_point_model_->getMountPoint(mount_point_box_->currentIndex());
  this->forceFormat((real_partition.fs != fs_type) ||
                    (mount_point == kMountPointRoot));
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
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
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

  usage_bar_ = new QProgressBar();
  usage_bar_->setObjectName("usage_bar");
  usage_bar_->setTextVisible(false);
  usage_bar_->setFixedSize(kProgressBarWidth, 8);

  QLabel* separator_label = new QLabel();
  separator_label->setObjectName("separator_label");
  separator_label->setFixedSize(560, 2);

  fs_label_ = new QLabel(tr(kTextFilesystem));
  fs_label_->setObjectName("fs_label");
  mount_point_label_ = new QLabel(tr(kTextMountPoint));
  mount_point_label_->setObjectName("mount_point_label");
  format_label_ = new QLabel(tr(kTextFormat));
  format_label_->setObjectName("format_label");

  fs_box_ = new TableComboBox();
  fs_box_->setObjectName("fs_box");
  fs_model_ = new FsModel(delegate_, this);
  fs_box_->setModel(fs_model_);

  mount_point_box_ = new TableComboBox();
  mount_point_box_->setObjectName("mount_point_box");
  mount_point_model_ = new MountPointModel(delegate_->allMountPoints(), this);
  mount_point_box_->setModel(mount_point_model_);

  format_check_box_ = new QCheckBox();
  format_check_box_->setObjectName("format_check_box");
  format_check_box_->setFixedWidth(20);

  QGridLayout* grid_layout = new QGridLayout();
  grid_layout->setHorizontalSpacing(20);
  grid_layout->setVerticalSpacing(20);
  grid_layout->setContentsMargins(0, 0, 0, 0);
  grid_layout->addWidget(fs_label_, 0, 0, Qt::AlignRight);
  grid_layout->addWidget(mount_point_label_, 1, 0, Qt::AlignRight);
  grid_layout->addWidget(fs_box_, 0, 1);
  grid_layout->addWidget(mount_point_box_, 1, 1);
  grid_layout->addWidget(format_check_box_, 2, 0, Qt::AlignRight);
  grid_layout->addWidget(format_label_, 2, 1);

  QVBoxLayout* grid_wrapper_layout = new QVBoxLayout();
  grid_wrapper_layout->setContentsMargins(0, 0, 0, 0);
  grid_wrapper_layout->setSpacing(0);
  grid_wrapper_layout->addLayout(grid_layout);
  grid_wrapper_layout->addStretch();

  QFrame* grid_frame = new QFrame();
  grid_frame->setObjectName("grid_frame");
  grid_frame->setContentsMargins(0, 0, 110, 0);
  grid_frame->setFixedSize(400, 300);
  grid_frame->setLayout(grid_wrapper_layout);

  cancel_button_ = new NavButton(tr(kTextCancel));
  ok_button_ = new NavButton(tr(kTextOk));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(os_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addWidget(name_frame, 0, Qt::AlignHCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addWidget(usage_bar_, 0, Qt::AlignHCenter);
  layout->addSpacing(20 + kMainLayoutSpacing);
  layout->addWidget(separator_label, 0, Qt::AlignHCenter);
  layout->addSpacing(20 + kMainLayoutSpacing);
  layout->addWidget(grid_frame, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(cancel_button_, 0, Qt::AlignHCenter);
  layout->addSpacing(30);
  layout->addWidget(ok_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setStyleSheet(ReadFile(":/styles/edit_partition_frame.css"));
}

void EditPartitionFrame::onFsChanged(int index) {
  const FsType fs_type = fs_model_->getFs(index);
  const bool visible = IsMountPointSupported(fs_type);

  mount_point_label_->setVisible(visible);
  mount_point_box_->setVisible(visible);

  this->updateFormatBoxState();
}

void EditPartitionFrame::onMountPointChanged(int index) {
  Q_UNUSED(index);
  this->updateFormatBoxState();
}

void EditPartitionFrame::onOkButtonClicked() {
  QString mount_point;
  if (mount_point_box_->isVisible()) {
    // Set mount_point only if mount_point_box_ is visible.
    mount_point =
        mount_point_model_->getMountPoint(mount_point_box_->currentIndex());
  }
  if(format_check_box_->isChecked()) {
    // Create an OperationFormat object.
    const FsType fs_type = fs_model_->getFs(fs_box_->currentIndex());
    delegate_->formatPartition(partition_, fs_type, mount_point);
    delegate_->refreshVisual();
  } else {
    // If FormatOperation is reverted.
    if (partition_.status == PartitionStatus::Format) {
      delegate_->unFormatPartition(partition_);
      // Reset partition status.
      partition_.status = PartitionStatus::Real;
    }

    // Create an OperationMountPoint object.
    if (partition_.mount_point != mount_point) {
      delegate_->updateMountPoint(partition_, mount_point);
    }
    delegate_->refreshVisual();
  }

  emit this->finished();
}

}  // namespace installer