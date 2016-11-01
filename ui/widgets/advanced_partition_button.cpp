// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/advanced_partition_button.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include "base/file_util.h"
#include "ui/delegates/partition_util.h"

namespace installer {

namespace {

QString GetImageByOsType(OsType os_type) {
  switch (os_type) {
    case OsType::Empty: {
      return QStringLiteral(":/images/driver_128.png");
    }
    case OsType::Linux: {
      return QStringLiteral(":/images/driver_linux_32.png");
    }
    case OsType::Mac: {
      return QStringLiteral(":/images/driver_mac_32.png");
    }
    case OsType::Unknown: {
      return QStringLiteral(":/images/driver_32.png");
    }
    case OsType::Windows: {
      return QStringLiteral(":/images/driver_windows_32.png");
    }
    default: {
      return QString();
    }
  }
}

}  // namespace

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

  QLabel* os_label = new QLabel();
  os_label->setObjectName(QStringLiteral("os_label"));
  os_label->setPixmap(GetImageByOsType(partition_.os));
  os_label->setFixedWidth(32);

  // partition label name
  QLabel* name_label = new QLabel();
  name_label->setObjectName(QStringLiteral("name_label"));
  // TODO(xushaohua): trim text
  if (partition_.type == PartitionType::Unallocated) {
    name_label->setText(tr("Freespace"));
  } else {
    if (!partition_.label.isEmpty()) {
      name_label->setText(partition_.label);
    } else {
      name_label->setText(GetPartitionName(partition_.path));
    }
  }
  name_label->setFixedWidth(64);

  // partition path
  QLabel* path_label = new QLabel();
  path_label->setObjectName(QStringLiteral("path_label"));
  // TODO(xushaohua): Move to partition_util.h
  if (partition_.type != PartitionType::Unallocated) {
    path_label->setText(
        QString("(%1)").arg(GetPartitionName(partition_.path)));
  }
  path_label->setFixedWidth(64);

  // partition space usage
  QLabel* usage_label = new QLabel();
  usage_label->setObjectName(QStringLiteral("usage_label"));
  usage_label->setText(GetPartitionUsage(partition_));
  usage_label->setFixedWidth(48);

  QProgressBar* usage_bar = new QProgressBar();
  usage_bar->setObjectName(QStringLiteral("usage_bar"));
  usage_bar->setTextVisible(false);
  usage_bar->setValue(int(GetPartitionUsageValue(partition_) * 100));
  usage_bar->setFixedSize(100, 6);

  // mount point
  QLabel* mount_point_label = new QLabel();
  mount_point_label->setObjectName(QStringLiteral("mount_point_label"));
  if (!partition_.mount_point.isEmpty()) {
    mount_point_label->setText(partition_.mount_point);
  }
  mount_point_label->setFixedWidth(64);
  mount_point_label->setText("/root");

  // tip
  QLabel* tip_label = new QLabel();
  tip_label->setObjectName(QStringLiteral("tip_label"));
  // TODO(xushaohua): Read root partition.
  tip_label->setFixedWidth(64);
  tip_label->setText("Install here");

  // filesystem name
  QLabel* fs_label = new QLabel();
  fs_label->setObjectName(QStringLiteral("fs_label"));
  if (partition_.type != PartitionType::Unallocated) {
    fs_label->setText(GetFsTypeName(partition_.fs));
  }
  fs_label->setFixedWidth(30);


  QFrame* control_button_wrapper = new QFrame();
  control_button_wrapper->setObjectName("control_button_wrapper");
  control_button_wrapper->setContentsMargins(0, 0, 0, 0);
  control_button_wrapper->setFixedSize(18, 18);

  control_button_ = new PointerButton(control_button_wrapper);
  control_button_->setObjectName(QStringLiteral("control_button"));
  control_button_->setFlat(true);
  control_button_->setFixedSize(18, 18);
  control_button_->hide();

  // TODO(xushaohua): Use fixed layout instead.
  QHBoxLayout* layout = new QHBoxLayout();
  layout->addSpacing(20);
  layout->addWidget(os_label);
  layout->addWidget(name_label);
  layout->addWidget(path_label);
  layout->addStretch();
  layout->addWidget(usage_label);
  layout->addWidget(usage_bar);
  layout->addStretch();
  layout->addWidget(mount_point_label);
  layout->addStretch();
  layout->addWidget(tip_label);
  layout->addStretch();
  layout->addWidget(fs_label);
  layout->addStretch();
  layout->addWidget(control_button_wrapper);
  layout->addSpacing(15);

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