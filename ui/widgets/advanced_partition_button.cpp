// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/advanced_partition_button.h"

#include <QHBoxLayout>
#include <QLabel>

#include "base/file_util.h"
#include "ui/delegates/partition_util.h"

namespace installer {

AdvancedPartitionButton::AdvancedPartitionButton(const Partition& partition,
                                                 QWidget* parent)
    : PointerButton(parent),
      partition_(partition),
      editable_(false) {
  this->setObjectName(QStringLiteral("advanced_partition_button"));

  this->initUI();
  this->initConnections();
}

void AdvancedPartitionButton::setEditable(bool editable) {
  this->editable_ = editable;
  this->updateStatus();
}

void AdvancedPartitionButton::initConnections() {
  connect(control_button_, &QPushButton::clicked,
          this, &AdvancedPartitionButton::onControlButtonClicked);
  connect(this, &QPushButton::toggled,
          this, &AdvancedPartitionButton::onToggled);
}

void AdvancedPartitionButton::initUI() {
  // os-prober logo
  // filesystem type
  // partition label
  partition_label_ = new QLabel();
  partition_label_->setObjectName(QStringLiteral("partition_label"));
  if (partition_.type == PartitionType::Unallocated) {
    partition_label_->setText(tr("Freespace"));
  } else {
    if (!partition_.label.isEmpty()) {
      partition_label_->setText(partition_.label);
    } else {
      partition_label_->setText(GetPartitionName(partition_.path));
    }
  }

  partition_path_label_ = new QLabel();
  partition_path_label_->setObjectName(QStringLiteral("partition_path_label"));
  // TODO(xushaohua): Move to partition_util.h
  if (partition_.type != PartitionType::Unallocated) {
    partition_path_label_->setText(
        QString("(%1)").arg(GetPartitionName(partition_.path)));
  }

  // partition space usage
  usage_label_ = new QLabel();
  usage_label_->setObjectName(QStringLiteral("usage_label"));
  usage_label_->setText(GetPartitionUsage(partition_));

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
  fs_type_label_->setObjectName(QStringLiteral("fs_type_label"));
  if (partition_.type != PartitionType::Unallocated) {
    fs_type_label_->setText(GetFsTypeName(partition_.fs));
  }

  control_button_ = new PointerButton();
  control_button_->setObjectName(QStringLiteral("control_button"));
  control_button_->setFixedSize(18, 18);
  control_button_->hide();

  // TODO(xushaohua): Use fixed layout instead.
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addSpacing(20);
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
  this->setFixedHeight(45);
  this->setCheckable(true);
  this->setChecked(false);
  this->setFlat(true);
  this->setStyleSheet(
      ReadTextFileContent(":/styles/advanced_partition_button.css"));
}

void AdvancedPartitionButton::updateStatus() {
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

void AdvancedPartitionButton::onControlButtonClicked() {
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

void AdvancedPartitionButton::onToggled() {
  this->updateStatus();
}

}  // namespace installer