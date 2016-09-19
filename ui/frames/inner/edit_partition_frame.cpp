// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/edit_partition_frame.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/models/fs_model.h"
#include "ui/models/mount_point_model.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/table_combo_box.h"
#include "ui/widgets/table_item_label.h"
#include "ui/widgets/title_label.h"

namespace ui {

EditPartitionFrame::EditPartitionFrame(PartitionDelegate* delegate,
                                       QWidget* parent)
    : QFrame(parent),
      delegate_(delegate),
      partition_() {
  this->setObjectName(QStringLiteral("edit_partition_frame"));

  this->initUI();
  this->initConnections();
}

void EditPartitionFrame::setPartition(const partman::Partition& partition) {
  partition_ = partition;

  // Reset fs index.
  int fs_index = fs_model_->index(partition.fs);
  if (fs_index == -1) {
    fs_index = 0;
  }
  fs_box_->setCurrentIndex(fs_index);

  // Reset mount point box.
  int mount_point_index = mount_point_model_->index(partition.mount_point);
  if (mount_point_index == -1) {
    mount_point_index = mount_point_model_->indexOfEmpty();
  }
  if (mount_point_index == -1) {
    mount_point_index = 0;
  }
  mount_point_box_->setCurrentIndex(mount_point_index);
}

void EditPartitionFrame::initConnections() {
  connect(fs_box_,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &EditPartitionFrame::onFsChanged);

  // Does nothing when cancel-button is clicked.
  connect(cancel_button_, &QPushButton::clicked,
          this, &EditPartitionFrame::finished);
  connect(ok_button_, &QPushButton::clicked,
          this, &EditPartitionFrame::onOkButtonClicked);
}

void EditPartitionFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Edit Disk"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Please make sure you have backed up important data, then "
         "select the disk  to install"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  TableItemLabel* fs_label = new TableItemLabel(tr("Filesystem"));
  mount_point_label_ = new TableItemLabel(tr("Mount point"));
  format_label_ = new TableItemLabel(tr("Format the partition"));

  fs_box_ = new TableComboBox();
  fs_model_ = new FsModel(delegate_, this);
  fs_box_->setModel(fs_model_);

  mount_point_box_ = new TableComboBox();
  mount_point_model_ = new MountPointModel(delegate_, this);
  mount_point_box_->setModel(mount_point_model_);

  format_check_box_ = new QCheckBox();
  format_check_box_->setFixedWidth(20);

  QGridLayout* grid_layout = new QGridLayout();
  grid_layout->addWidget(fs_label, 0, 0);
  grid_layout->addWidget(mount_point_label_, 1, 0);
  grid_layout->addWidget(fs_box_, 0, 1);
  grid_layout->addWidget(mount_point_box_, 1, 1);
  grid_layout->addWidget(format_check_box_, 2, 0);
  grid_layout->addWidget(format_label_, 2, 1);

  QHBoxLayout* grid_wrapper_layout = new QHBoxLayout();
  grid_wrapper_layout->addStretch();
  grid_wrapper_layout->addLayout(grid_layout);
  grid_wrapper_layout->addStretch();

  cancel_button_ = new NavButton(tr("Cancel"));
  QHBoxLayout* cancel_layout = new QHBoxLayout();
  cancel_layout->addWidget(cancel_button_);

  ok_button_ = new NavButton(tr("OK"));
  QHBoxLayout* ok_layout = new QHBoxLayout();
  ok_layout->addWidget(ok_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(grid_wrapper_layout);
  layout->addStretch();
  layout->addLayout(cancel_layout);
  layout->addLayout(ok_layout);

  this->setLayout(layout);
}

void EditPartitionFrame::onFsChanged(int index) {
  const partman::FsType fs_type = fs_model_->getFs(index);
  const bool visible = SupportMountPoint(fs_type);

  mount_point_label_->setVisible(visible);
  mount_point_box_->setVisible(visible);
  format_label_->setVisible(visible);
  format_check_box_->setVisible(visible);

  const bool checked = !(fs_type == partman::FsType::Empty ||
                         fs_type == partman::FsType::Unknown ||
                         fs_type == partition_.fs);
  format_check_box_->setChecked(checked);
}

void EditPartitionFrame::onOkButtonClicked() {

  const QString mount_point = mount_point_box_->currentText();
  if(format_check_box_->isChecked()) {
    // Create an OperationFormat object.
    const partman::FsType fs_type = fs_model_->getFs(fs_box_->currentIndex());
    delegate_->formatPartition(partition_, fs_type, mount_point);
  } else if (mount_point != partition_.mount_point) {
    // Only create an OperationMountPoint object.
    delegate_->updateMountPoint(partition_, mount_point);
  }
  mount_point_model_->useMountPoint(mount_point);


  emit this->finished();
}

}  // namespace ui