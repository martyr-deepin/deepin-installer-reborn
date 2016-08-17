// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/simple_partition_button.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/utils/partition_util.h"
#include "ui/widgets/partition_usage_bar.h"

namespace ui {

namespace {

QString GetImageByOsType(service::OsType os_type) {
  switch (os_type) {
    case service::OsType::Empty: {
      return QStringLiteral(":/images/drive-harddisk-128px.png");
    }
    case service::OsType::Linux: {
      return QStringLiteral(":/images/drive-harddisk-linux-128px.png");
    }
    case service::OsType::Mac: {
      return QStringLiteral(":/images/drive-harddisk-mac-128px.png");
    }
    case service::OsType::Unknown: {
      return QStringLiteral(":/images/drive-harddisk-128px.png");
    }
    case service::OsType::Windows: {
      return QStringLiteral(":/images/drive-harddisk-windows-128px.png");
    }
  }
  return QString();
}

}  // namespace

SimplePartitionButton::SimplePartitionButton(
    const service::Partition& partition, QWidget* parent)
    : FlatButton(parent),
      partition_(partition) {
  this->setObjectName(QStringLiteral("simple_partition_button"));

  this->initUI();
  this->initConnections();
}

void SimplePartitionButton::initConnections() {
  connect(this, &QPushButton::toggled,
          this, &SimplePartitionButton::onButtonToggled);
}

void SimplePartitionButton::initUI() {
  fs_label_ = new QLabel();
  fs_label_->setObjectName(QStringLiteral("fs_label"));
  fs_label_->setFixedSize(120, 120);
  QPixmap os_icon(GetImageByOsType(partition_.os));
  fs_label_->setPixmap(os_icon);
  QHBoxLayout* fs_layout = new QHBoxLayout();
  fs_layout->addWidget(fs_label_);

  QLabel* path_label = new QLabel();
  if (partition_.label.isEmpty()) {
    path_label->setText(partition_.path);
  } else {
    // TODO(xushaohua): trim text.
    path_label->setText(
        QString("%1(%2)").arg(partition_.label).arg(partition_.path));
  }
  path_label->setObjectName(QStringLiteral("path_label"));
  path_label->setAlignment(Qt::AlignCenter);

  QLabel* usage_label = new QLabel();
  usage_label->setText(
      GetPartitionUsage(partition_.freespace, partition_.length));
  usage_label->setObjectName(QStringLiteral("usage_label"));
  usage_label->setAlignment(Qt::AlignCenter);

  PartitionUsageBar* usage_bar = new PartitionUsageBar(partition_.freespace,
                                                       partition_.length);
  QHBoxLayout* usage_bar_layout = new QHBoxLayout();
  usage_bar_layout->addWidget(usage_bar);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignCenter);
  layout->addLayout(fs_layout);
  layout->addWidget(path_label);
  layout->addWidget(usage_label);
  layout->addLayout(usage_bar_layout);

  this->setLayout(layout);

  this->setFixedSize(220, 220);
  this->setCheckable(true);
  this->setStyleSheet(base::ReadTextFileContent(
      QStringLiteral(":/styles/simple_partition_button.css")));
}

void SimplePartitionButton::onButtonToggled() {
  if (this->isChecked()) {
    QPixmap pixmap(QStringLiteral(":/images/drive-install-here-128px.png"));
    fs_label_->setPixmap(pixmap);
  } else {
    QPixmap pixmap(GetImageByOsType(partition_.os));
    fs_label_->setPixmap(pixmap);
  }
}

}  // namespace ui
