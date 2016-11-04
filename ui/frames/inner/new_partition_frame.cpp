// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/new_partition_frame.h"

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QStandardItemModel>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/models/fs_model.h"
#include "ui/models/mount_point_model.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/partition_size_slider.h"
#include "ui/widgets/table_combo_box.h"
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
  this->setObjectName("new_partition_frame");

  this->initUI();
  this->initConnections();
}

void NewPartitionFrame::setPartition(const Partition& partition) {
  partition_ = partition;

  const bool primary_ok = delegate_->canAddPrimary(partition);
  const bool logical_ok = delegate_->canAddLogical(partition);
  QStandardItemModel* type_model =
      static_cast<QStandardItemModel*>(type_box_->model());
  Q_ASSERT(type_model != nullptr);
  type_model->item(0)->setEnabled(primary_ok);
  type_model->item(1)->setEnabled(logical_ok);

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
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("Create a new partition and define the type and size"));

  QLabel* os_label = new QLabel();
  os_label->setObjectName("os_label");
  os_label->setPixmap(GetOsTypeLargeIcon(partition_.os));

  QLabel* separator_label = new QLabel();
  separator_label->setObjectName("separator_label");
  separator_label->setFixedSize(560, 2);

  QLabel* type_label = new QLabel(tr("Type"));
  type_label->setObjectName("type_label");
  QLabel* location_label = new QLabel(tr("Location"));
  location_label->setObjectName("location_label");
  QLabel* fs_label = new QLabel(tr("Filesystem"));
  fs_label->setObjectName("fs_label");
  QLabel* mount_point_label = new QLabel(tr("Mount point"));
  mount_point_label->setObjectName("mount_point_label");
  QLabel* size_label = new QLabel(tr("Size"));
  size_label->setObjectName("size_label");

  type_box_ = new TableComboBox();
  // TODO(xushaohua): Add type model
  type_box_->addItems({tr(kTypePrimary), tr(kTypeLogical)});

  alignment_box_ = new TableComboBox();
  // TODO(xushaohua): Add model
  alignment_box_->addItems({tr("Start"), tr("End")});

  fs_box_ = new TableComboBox();
  fs_model_ = new FsModel(delegate_, fs_box_);
  fs_box_->setModel(fs_model_);

  mount_point_box_ = new TableComboBox();
  mount_point_model_ = new MountPointModel(delegate_, mount_point_box_);
  mount_point_box_->setModel(mount_point_model_);

  size_slider_ = new PartitionSizeSlider();
  size_slider_->setFixedWidth(mount_point_box_->width());

  QGridLayout* grid_layout = new QGridLayout();
  grid_layout->setContentsMargins(0, 0, 0, 0);
  grid_layout->setHorizontalSpacing(20);
  grid_layout->setVerticalSpacing(20);
  grid_layout->addWidget(type_label, 0, 0, Qt::AlignRight);
  grid_layout->addWidget(location_label, 1, 0, Qt::AlignRight);
  grid_layout->addWidget(fs_label, 2, 0, Qt::AlignRight);
  grid_layout->addWidget(mount_point_label, 3, 0, Qt::AlignRight);
  grid_layout->addWidget(size_label, 4, 0, Qt::AlignRight);
  grid_layout->addWidget(type_box_, 0, 1);
  grid_layout->addWidget(alignment_box_, 1, 1);
  grid_layout->addWidget(fs_box_, 2, 1);
  grid_layout->addWidget(mount_point_box_, 3, 1);
  grid_layout->addWidget(size_slider_, 4, 1);

  QFrame* grid_frame = new QFrame();
  grid_frame->setObjectName("grid_frame");
  grid_frame->setContentsMargins(0, 0, 0, 0);
  grid_frame->setLayout(grid_layout);
  grid_frame->setFixedWidth(360);

  cancel_button_ = new NavButton(tr("Cancel"));
  create_button_ = new NavButton(tr("OK"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(os_label, 0, Qt::AlignCenter);
  layout->addSpacing(10);
  layout->addWidget(separator_label, 0, Qt::AlignHCenter);
  layout->addSpacing(10);
  layout->addWidget(grid_frame, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(cancel_button_, 0, Qt::AlignCenter);
  layout->addWidget(create_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/new_partition_frame.css"));
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