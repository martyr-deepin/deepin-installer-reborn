// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/new_partition_frame.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/table_combo_box.h"
#include "ui/widgets/table_item_label.h"
#include "ui/widgets/title_label.h"

namespace ui {

NewPartitionFrame::NewPartitionFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("new_partition_frame"));

  this->initUI();
  this->initConnections();
}

void NewPartitionFrame::setPath(const QString& partition_path) {
  Q_UNUSED(partition_path);
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
  type_box_->addItems({tr("Primary"), tr("Logical")});
  location_box_ = new TableComboBox();
  location_box_->addItems({tr("Start"), tr("End")});
  fs_box_ = new TableComboBox();
  // TODO(xushaohua): Convert fs_type to string.
  fs_box_->addItems({"Ext4", "Ext3", "LinuxSwap"});
  mount_point_box_ = new TableComboBox();
  mount_point_box_->addItems({"/", "/boot", "/home"});
  size_box_ = new TableComboBox();

  QGridLayout* grid_layout = new QGridLayout();
  grid_layout->addWidget(type_label, 0, 0);
  grid_layout->addWidget(location_label, 1, 0);
  grid_layout->addWidget(fs_label, 2, 0);
  grid_layout->addWidget(mount_point_label, 3, 0);
  grid_layout->addWidget(size_label, 4, 0);
  grid_layout->addWidget(type_box_, 0, 1);
  grid_layout->addWidget(location_box_, 1, 1);
  grid_layout->addWidget(fs_box_, 2, 1);
  grid_layout->addWidget(mount_point_box_, 3, 1);
  grid_layout->addWidget(size_box_, 4, 1);
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
  // TODO(xushaohua): Create an OperationNew object
  emit this->finished();
}

}  // namespace ui