// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/advanced_partition_item.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>

#include "partman/partition_manager.h"
#include "ui/frames/delegates/partition_util.h"
#include "ui/widgets/flat_button.h"
#include "ui/widgets/partition_usage_bar.h"

namespace ui {

AdvancedPartitionItem::AdvancedPartitionItem(
    const partman::Partition& partition, QWidget* parent)
    : QFrame(parent),
      partition_(partition),
      edit_delete_button_state_(EditDeleteButtonState::Hide) {
  this->setObjectName(QStringLiteral("advanced_partition_item"));

  this->initUI();
  this->initConnections();
}

void AdvancedPartitionItem::setEditDeleteButtonState(
    EditDeleteButtonState state) {
  switch (state) {
    case EditDeleteButtonState::Delete: {
      edit_delete_button_->setIcon(QIcon(":/images/delete_partition.png"));
      edit_delete_button_->show();
      break;
    }
    case EditDeleteButtonState::Edit: {
      edit_delete_button_->setIcon(QIcon(":/images/edit_partition.png"));
      edit_delete_button_->show();
      break;
    }
    case EditDeleteButtonState::Hide: {
      edit_delete_button_->hide();
    }
  }
  edit_delete_button_state_ = state;
}

void AdvancedPartitionItem::setMountPoint(const QString& mount_point) {
  mount_point_label_->setText(mount_point);
}

void AdvancedPartitionItem::setFilesystemType(const QString& fs) {
  fs_type_label_->setText(fs);
}

void AdvancedPartitionItem::initConnections() {
  connect(edit_delete_button_, &QPushButton::clicked,
          this, &AdvancedPartitionItem::onEditDeleteButtonClicked);
}

void AdvancedPartitionItem::initUI() {
  // os-prober logo
  // filesystem type
  // partition label
  partition_label_ = new QLabel();
  if (!partition_.label.isEmpty()) {
    partition_label_->setText(partition_.label);
  } else {
    partition_label_->setText(GetPartitionName(partition_.path));
  }
  partition_label_->setObjectName(QStringLiteral("partition_label"));

  partition_path_label_ = new QLabel();
  partition_path_label_->setText(
      QString("(%1)").arg(GetPartitionName(partition_.path)));
  partition_path_label_->setObjectName(QStringLiteral("partition_path_label"));

  // partition space usage
  usage_label_ = new QLabel();
  usage_label_->setText(GetPartitionUsage(partition_.freespace,
                                          partition_.length));
  usage_label_->setObjectName(QStringLiteral("usage_label"));
  PartitionUsageBar* usage_bar = new PartitionUsageBar(partition_.freespace,
                                                       partition_.length);

  // mount point
  mount_point_label_ = new QLabel();
  mount_point_label_->setObjectName(QStringLiteral("mount_point_label"));

  // tip
  tip_label_ = new QLabel();
  tip_label_->setObjectName(QStringLiteral("tip_label"));

  // filesystem name
  fs_type_label_ = new QLabel();
  fs_type_label_->setText(GetFsTypeName(partition_.fs));
  fs_type_label_->setObjectName(QStringLiteral("fs_type_label"));

  // edit/delete button
  edit_delete_button_ = new FlatButton();
  edit_delete_button_->setFixedSize(18, 18);
  this->setEditDeleteButtonState(EditDeleteButtonState::Delete);

  // TODO(xuhaohua): Use fixed layout instead.
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(partition_label_);
  layout->addWidget(partition_path_label_);
  layout->addStretch();
  layout->addWidget(usage_label_);
  layout->addWidget(usage_bar);
  layout->addStretch();
  layout->addWidget(mount_point_label_);
  layout->addStretch();
  layout->addWidget(tip_label_);
  layout->addStretch();
  layout->addWidget(fs_type_label_);
  layout->addStretch();
  layout->addWidget(edit_delete_button_);

  this->setLayout(layout);

  this->setFixedSize(480, 36);
}

void AdvancedPartitionItem::onEditDeleteButtonClicked() {
  switch (edit_delete_button_state_) {
    case EditDeleteButtonState::Delete: {
      emit this->deletePartitionTriggered();
      break;
    }
    case EditDeleteButtonState::Edit: {
      emit this->editPartitionTriggered();
      break;
    }
    default: {
      break;
    }
  }
}

}  // namespace ui