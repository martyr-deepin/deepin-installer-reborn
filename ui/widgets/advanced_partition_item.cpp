// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/advanced_partition_item.h"

#include <QHBoxLayout>
#include <QLabel>

#include "ui/delegates/partition_util.h"

namespace installer {

AdvancedPartitionItem::AdvancedPartitionItem(
    const Partition& partition, QWidget* parent)
    : FlatButton(parent),
      partition_(partition),
      editable_(false) {
  this->setObjectName(QStringLiteral("advanced_partition_item"));

  this->initUI();
  this->initConnections();
}

void AdvancedPartitionItem::setEditable(bool editable) {
  this->editable_ = editable;
  this->updateStatus();
}

void AdvancedPartitionItem::initConnections() {
  connect(control_button_, &QPushButton::clicked,
          this, &AdvancedPartitionItem::onControlButtonClicked);
  connect(this, &QPushButton::toggled,
          this, &AdvancedPartitionItem::onToggled);
}

void AdvancedPartitionItem::initUI() {
  // os-prober logo
  // filesystem type
  // partition label
  partition_label_ = new QLabel();
  if (partition_.type == PartitionType::Unallocated) {
    partition_label_->setText(tr("Freespace"));
  } else {
    if (!partition_.label.isEmpty()) {
      partition_label_->setText(partition_.label);
    } else {
      partition_label_->setText(GetPartitionName(partition_.path));
    }
  }

  partition_label_->setObjectName(QStringLiteral("partition_label"));

  partition_path_label_ = new QLabel();
  if (partition_.type != PartitionType::Unallocated) {
    partition_path_label_->setText(
        QString("(%1)").arg(GetPartitionName(partition_.path)));
  }
  partition_path_label_->setObjectName(QStringLiteral("partition_path_label"));

  // partition space usage
  usage_label_ = new QLabel();
  qint64 freespace;
  qint64 total;
  if (partition_.type == PartitionType::Unallocated || partition_.length <= 0) {
    total = partition_.getByteLength();
    freespace = total;
  } else {
    freespace = partition_.freespace;
    total = partition_.length;
  }
  usage_label_->setText(GetPartitionUsage(freespace, total));
  usage_label_->setObjectName(QStringLiteral("usage_label"));

  // mount point
  mount_point_label_ = new QLabel();
  mount_point_label_->setObjectName(QStringLiteral("mount_point_label"));
  if (!partition_.mount_point.isEmpty()) {
    mount_point_label_->setText(partition_.mount_point);
  }

  // tip
  tip_label_ = new QLabel();
  tip_label_->setObjectName(QStringLiteral("tip_label"));

  // filesystem name
  fs_type_label_ = new QLabel();
  if (partition_.type != PartitionType::Unallocated) {
    fs_type_label_->setText(GetFsTypeName(partition_.fs));
  }
  fs_type_label_->setObjectName(QStringLiteral("fs_type_label"));

  control_button_ = new FlatButton();
  control_button_->setFixedSize(18, 18);
  control_button_->hide();

  // TODO(xushaohua): Use fixed layout instead.
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(partition_label_);
  layout->addWidget(partition_path_label_);
  layout->addStretch();
  layout->addWidget(usage_label_);
  layout->addStretch();
  layout->addWidget(mount_point_label_);
  layout->addStretch();
  layout->addWidget(tip_label_);
  layout->addStretch();
  layout->addWidget(fs_type_label_);
  layout->addStretch();
  layout->addWidget(control_button_);

  this->setLayout(layout);
  this->setFixedSize(480, 36);
  this->setCheckable(true);
  this->setChecked(false);
}

void AdvancedPartitionItem::updateStatus() {
  if (editable_) {
    if (partition_.type == PartitionType::Unallocated) {
      control_status_ = ControlStatus::New;
      control_button_->setIcon(
          QIcon(QStringLiteral(":/images/new_partition.png")));
    } else {
      control_status_ = ControlStatus::Delete;
      control_button_->setIcon(
          QIcon(QStringLiteral(":/images/delete_partition.png")));
    }
  } else {
    if (partition_.type != PartitionType::Unallocated &&
        this->isChecked()) {
      control_status_ = ControlStatus::Edit;
      control_button_->setIcon(
          QIcon(QStringLiteral(":/images/edit_partition.png")));
    } else {
      control_status_ = ControlStatus::Hide;
    }
  }

  control_button_->setVisible(control_status_ != ControlStatus::Hide);
}

void AdvancedPartitionItem::onControlButtonClicked() {
  switch (control_status_) {
    case ControlStatus::Delete: {
      emit this->deletePartitionTriggered(partition_);
      break;
    }
    case ControlStatus::Edit: {
      emit this->editPartitionTriggered(partition_);
      break;
    }
    case ControlStatus::New: {
      emit this->newPartitionTriggered(partition_);
      break;
    }
    default: {
      // Never reach here.
      break;
    }
  }
}

void AdvancedPartitionItem::onToggled() {
  this->updateStatus();
}

}  // namespace installer