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
#include "ui/frames/delegates/partition_delegate.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/table_combo_box.h"
#include "ui/widgets/table_item_label.h"
#include "ui/widgets/title_label.h"

namespace ui {

EditPartitionFrame::EditPartitionFrame(PartitionDelegate* delegate,
                                       QWidget* parent)
    : QFrame(parent),
      delegate_(delegate) {
  this->setObjectName(QStringLiteral("edit_partition_frame"));

  this->initUI();
  this->initConnections();
}

void EditPartitionFrame::setPath(const QString& partition_path) {
  Q_UNUSED(partition_path);
}

void EditPartitionFrame::initConnections() {
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
  TableItemLabel* mount_point_label = new TableItemLabel(tr("Mount point"));
  TableItemLabel* format_label = new TableItemLabel(tr("Format the partition"));

  fs_box_ = new TableComboBox();
  fs_box_->addItems({"Ext4", "Ext3"});
  mount_point_box_ = new TableComboBox();
  mount_point_box_->addItems({"/", "/boot"});

  QCheckBox* format_check_box = new QCheckBox();
  format_check_box->setFixedWidth(20);

  QGridLayout* grid_layout = new QGridLayout();
  grid_layout->addWidget(fs_label, 0, 0);
  grid_layout->addWidget(mount_point_label, 1, 0);
  grid_layout->addWidget(fs_box_, 0, 1);
  grid_layout->addWidget(mount_point_box_, 1, 1);
  grid_layout->addWidget(format_check_box, 2, 0);
  grid_layout->addWidget(format_label, 2, 1);

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

void EditPartitionFrame::onOkButtonClicked() {
  // TODO(xushaohua): Create an OperationModify object
  emit this->finished();
}

}  // namespace ui