// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/new_partition_frame.h"

#include <QDebug>
#include <QGridLayout>
#include <QLabel>

#include "ui/frames/consts.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/models/fs_model.h"
#include "ui/models/mount_point_model.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/partition_size_slider.h"
#include "ui/widgets/table_combo_box.h"
#include "ui/widgets/table_item_label.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTypePrimary[] = "Primary partition";
const char kTypeLogical[] = "Logical partition";

}  // namespace

NewPartitionFrame::NewPartitionFrame(PartitionDelegate* delegate,
                                     QWidget* parent)
    : QFrame(parent),
      delegate_(delegate),
      partition_() {
  this->setObjectName(QStringLiteral("new_partition_frame"));

  this->initUI();
  this->initConnections();
}

void NewPartitionFrame::setPartition(const Partition& partition) {
  partition_ = partition;

  const bool primary_ok = delegate_->canAddPrimary(partition);
  const bool logical_ok = delegate_->canAddLogical(partition);
  type_box_->clear();
  if (primary_ok) {
    type_box_->addItem(tr(kTypePrimary));
  }
  if (logical_ok) {
    type_box_->addItem(tr(kTypeLogical));
  }

  if (!primary_ok && !logical_ok) {
    qCritical() << "No more partition available!";
  }

  alignment_box_->setCurrentIndex(0);
  // TODO(xushaohua): Reset all status.

  size_slider_->setMaximum(partition.getByteLength());
}

void NewPartitionFrame::initConnections() {
  connect(cancel_button_, &QPushButton::clicked,
          this, &NewPartitionFrame::finished);
  connect(create_button_, &QPushButton::clicked,
          this, &NewPartitionFrame::onCreateButtonClicked);
}

void NewPartitionFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("New Partition"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Create a new partition and define the type and size"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  QLabel* fs_img_label = new QLabel();
  QPixmap fs_img(":/images/drive-harddisk-128px.png");
  fs_img_label->setPixmap(fs_img);
  fs_img_label->setFixedSize(fs_img.size());
  QHBoxLayout* fs_img_layout = new QHBoxLayout();
  fs_img_layout->addWidget(fs_img_label);

  TableItemLabel* type_label = new TableItemLabel(tr("Type"));
  TableItemLabel* location_label = new TableItemLabel(tr("Location"));
  TableItemLabel* fs_label = new TableItemLabel(tr("Filesystem"));
  TableItemLabel* mount_point_label = new TableItemLabel(tr("Mount point"));
  TableItemLabel* size_label = new TableItemLabel(tr("Size"));

  type_box_ = new TableComboBox();
  type_box_->addItems({tr(kTypePrimary), tr(kTypeLogical)});
  alignment_box_ = new TableComboBox();
  alignment_box_->addItems({tr("Start"), tr("End")});

  fs_box_ = new TableComboBox();
  fs_model_ = new FsModel(delegate_, fs_box_);
  fs_box_->setModel(fs_model_);

  mount_point_box_ = new TableComboBox();
  mount_point_model_ = new MountPointModel(delegate_, mount_point_box_);
  mount_point_box_->setModel(mount_point_model_);

  size_slider_ = new PartitionSizeSlider();

  QGridLayout* grid_layout = new QGridLayout();
  grid_layout->addWidget(type_label, 0, 0);
  grid_layout->addWidget(location_label, 1, 0);
  grid_layout->addWidget(fs_label, 2, 0);
  grid_layout->addWidget(mount_point_label, 3, 0);
  grid_layout->addWidget(size_label, 4, 0);
  grid_layout->addWidget(type_box_, 0, 1);
  grid_layout->addWidget(alignment_box_, 1, 1);
  grid_layout->addWidget(fs_box_, 2, 1);
  grid_layout->addWidget(mount_point_box_, 3, 1);
  grid_layout->addWidget(size_slider_, 4, 1);
  QHBoxLayout* grid_wrapper_layout = new QHBoxLayout();;
  grid_wrapper_layout->addStretch();
  grid_wrapper_layout->addLayout(grid_layout);
  grid_wrapper_layout->addStretch();

  cancel_button_ = new NavButton(tr("Cancel"));
  QHBoxLayout* cancel_layout = new QHBoxLayout();
  cancel_layout->addWidget(cancel_button_);

  create_button_ = new NavButton(tr("OK"));
  QHBoxLayout* create_layout = new QHBoxLayout();
  create_layout->addWidget(create_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addLayout(fs_img_layout);
  layout->addStretch();
  layout->addLayout(grid_wrapper_layout);
  layout->addStretch();
  layout->addLayout(cancel_layout);
  layout->addLayout(create_layout);

  this->setLayout(layout);
}

void NewPartitionFrame::onCreateButtonClicked() {
  // FIXME(xushaohua): Check type box flag
  const bool is_primary = (type_box_->currentIndex() == 0);
  const PartitionType partition_type = is_primary ? PartitionType::Normal :
                                                    PartitionType::Logical;
  const bool align_start = (alignment_box_->currentIndex() == 0);
  const FsType fs_type = fs_model_->getFs(fs_box_->currentIndex());
  const int mp_index = mount_point_box_->currentIndex();
  const QString mount_point = mount_point_model_->getMountPoint(mp_index);
  // TODO(xushaohua): Calculate exact sectors
  const qint64 total_sectors = size_slider_->value() / partition_.sector_size;
  delegate_->createPartition(partition_, partition_type, align_start, fs_type,
                             mount_point, total_sectors);

  emit this->finished();
}

}  // namespace installer