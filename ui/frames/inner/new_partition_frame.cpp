// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/new_partition_frame.h"

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QStandardItemModel>
#include <QtCore/QEvent>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"
#include "ui/models/fs_model.h"
#include "ui/models/mount_point_model.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/partition_size_slider.h"
#include "ui/widgets/table_combo_box.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "New Partition";
const char kTextComment[] =
    "Create a new partition and define the type and size";
const char kTextType[] = "Type";
const char kTextAlignment[] = "Location";
const char kTextFilesystem[] = "Filesystem";
const char kTextMountPoint[] = "Mount point";
const char kTextSize[] = "Size";
const char kTypePrimary[] = "Primary partition";
const char kTypeLogical[] = "Logical partition";
const char kTextStart[] = "Start";
const char kTextEnd[] = "End";
const char kTextCancel[] = "Cancel";
const char kTextOk[] = "OK";

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
  fs_box_->setCurrentIndex(0);
  mount_point_box_->setCurrentIndex(0);
  size_slider_->setMaximum(partition.getByteLength());
}

void NewPartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
    type_label_->setText(tr(kTextType));
    alignment_label_->setText(tr(kTextAlignment));
    fs_label_->setText(tr(kTextFilesystem));
    mount_point_label_->setText(tr(kTextMountPoint));
    size_label_->setText(tr(kTextSize));
    type_box_->clear();
    type_box_->addItems({tr(kTypePrimary), tr(kTypeLogical)});
    alignment_box_->clear();
    alignment_box_->addItems({tr(kTextStart), tr(kTextEnd)});
    cancel_button_->setText(tr(kTextCancel));
    create_button_->setText(tr(kTextOk));
  } else {
    QFrame::changeEvent(event);
  }
}

void NewPartitionFrame::initConnections() {
  connect(cancel_button_, &QPushButton::clicked,
          this, &NewPartitionFrame::finished);
  connect(create_button_, &QPushButton::clicked,
          this, &NewPartitionFrame::onCreateButtonClicked);
}

void NewPartitionFrame::initUI() {
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(kTextComment);
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  QLabel* os_label = new QLabel();
  os_label->setObjectName("os_label");
  os_label->setPixmap(GetOsTypeLargeIcon(partition_.os));

  QLabel* separator_label = new QLabel();
  separator_label->setObjectName("separator_label");
  separator_label->setFixedSize(560, 2);

  type_label_ = new QLabel(tr(kTextType));
  type_label_->setObjectName("type_label");
  alignment_label_ = new QLabel(tr(kTextAlignment));
  alignment_label_->setObjectName("alignment_label");
  fs_label_ = new QLabel(tr(kTextFilesystem));
  fs_label_->setObjectName("fs_label");
  mount_point_label_ = new QLabel(tr(kTextMountPoint));
  mount_point_label_->setObjectName("mount_point_label");
  size_label_ = new QLabel(tr(kTextSize));
  size_label_->setObjectName("size_label");

  type_box_ = new TableComboBox();
  type_box_->addItems({tr(kTypePrimary), tr(kTypeLogical)});

  alignment_box_ = new TableComboBox();
  alignment_box_->addItems({tr(kTextStart), tr(kTextEnd)});

  fs_box_ = new TableComboBox();
  fs_model_ = new FsModel(delegate_, fs_box_);
  fs_box_->setModel(fs_model_);

  mount_point_box_ = new TableComboBox();
  mount_point_model_ = new MountPointModel(delegate_->allMountPoints(),
                                           mount_point_box_);
  mount_point_box_->setModel(mount_point_model_);

  size_slider_ = new PartitionSizeSlider();
  size_slider_->setFixedWidth(mount_point_box_->width());

  QGridLayout* grid_layout = new QGridLayout();
  grid_layout->setContentsMargins(0, 0, 0, 0);
  grid_layout->setHorizontalSpacing(20);
  grid_layout->setVerticalSpacing(20);
  grid_layout->addWidget(type_label_, 0, 0, Qt::AlignRight);
  grid_layout->addWidget(alignment_label_, 1, 0, Qt::AlignRight);
  grid_layout->addWidget(fs_label_, 2, 0, Qt::AlignRight);
  grid_layout->addWidget(mount_point_label_, 3, 0, Qt::AlignRight);
  grid_layout->addWidget(size_label_, 4, 0, Qt::AlignRight);
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

  cancel_button_ = new NavButton(tr(kTextCancel));
  create_button_ = new NavButton(tr(kTextOk));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
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