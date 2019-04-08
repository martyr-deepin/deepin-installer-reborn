/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "partition_table_warning_frame.h"

#include <QDebug>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/expanded_nav_button.h"
#include "ui/widgets/title_label.h"
#include "ui/utils/widget_util.h"

namespace installer {

PartitionTableWarningFrame::PartitionTableWarningFrame(QWidget* parent)
    : QFrame(parent),
      device_path_() {
  this->setObjectName("partition_table_warning_frame");

  this->initUI();
  this->initConnections();
}

QString PartitionTableWarningFrame::devicePath() const {
  return device_path_;
}

void PartitionTableWarningFrame::setDevicePath(const QString& device_path) {
  device_path_ = device_path;
}

void PartitionTableWarningFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Warning"));
    comment_label_->setText(
        tr("Unable to install directly due to EFI motherboard boot loader "
           "and MBR disk detected, please select one of the solutions below "
           "to continue."));
    list_title1_->setText(QString("A.%1").arg(tr("Disable UEFI")));
    list_item1_->setText(
        QString("1.%1\n2.%2")
            .arg(tr("Reboot, enter BIOS, and disable UEFI"))
            .arg(tr("Exit BIOS, and enter deepin to install")));
    list_title2_->setText(QString("B.%1").arg(tr("Format the disk")));
    list_item2_->setText(
        QString("1.%1\n2.%2")
            .arg(tr("Make a backup of all your data to avoid data loss"))
            .arg(tr("Please double check if you have backed up all data, "
                    "then reboot to enter this interface")));
    list_title3_->setText(tr("Continue"));
    list_item3_->setText(
        QString("1.%1\n2.%2")
            .arg(tr("Make sure you have backed up all data before proceeding"))
            .arg(tr("Continuing installation will format your disk")));
    reject_button_->setText(tr("Reboot"));
    accept_button_->setText(tr("Continue"));
    cancel_button_->setText(tr("Back"));
  } else {
    QFrame::changeEvent(event);
  }
}

void PartitionTableWarningFrame::initConnections() {
  connect(reject_button_, &QPushButton::clicked,
          this, &PartitionTableWarningFrame::reboot);
  connect(accept_button_, &QPushButton::clicked,
          this, &PartitionTableWarningFrame::onConfirmButtonClicked);
  connect(cancel_button_, &QPushButton::clicked,
          this, &PartitionTableWarningFrame::canceled);
}

void PartitionTableWarningFrame::initUI() {
  QLabel* warning_label = new QLabel();
  warning_label->setPixmap(installer::renderPixmap(":/images/warning.svg"));
  title_label_ = new TitleLabel(tr("Warning"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->setContentsMargins(0, 0, 0, 0);
  title_layout->setSpacing(0);
  title_layout->addStretch();
  title_layout->addWidget(warning_label);
  title_layout->addSpacing(8);
  title_layout->addWidget(title_label_);
  title_layout->addStretch();

  comment_label_ = new CommentLabel(
      tr("Unable to install directly due to EFI motherboard boot loader "
         "and MBR disk detected, please select one of the solutions below "
         "to continue."));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  list_title1_ = new QLabel(QString("A.%1").arg(tr("Disable UEFI")), this);
  list_title1_->setObjectName("list_title1");
  list_item1_ = new QLabel(
      QString("1.%1\n2.%2")
          .arg(tr("Reboot, enter BIOS, and disable UEFI"))
          .arg(tr("Exit BIOS, and enter deepin to install")),
      this);
  list_item1_->setObjectName("list_item1");
  list_item1_->setWordWrap(true);
  list_title2_ = new QLabel(QString("B.%1").arg(tr("Format the disk")), this);
  list_title2_->setObjectName("list_title2");
  list_item2_ = new QLabel(
      QString("1.%1\n2.%2")
          .arg(tr("Please make a backup of all your data to avoid data loss"))
          .arg(tr("Please double check if all the data were made a backup, "
                  "and reboot to enter this interface")),
      this);
  list_item2_->setObjectName("list_item2");
  list_item2_->setWordWrap(true);
  QVBoxLayout* left_frame_layout = new QVBoxLayout();
  left_frame_layout->setContentsMargins(20, 20, 20, 20);
  left_frame_layout->setSpacing(0);
  left_frame_layout->addWidget(list_title1_);
  left_frame_layout->addWidget(list_item1_);
  left_frame_layout->addStretch();
  left_frame_layout->addSpacing(5);
  left_frame_layout->addWidget(list_title2_);
  left_frame_layout->addWidget(list_item2_);
  left_frame_layout->addStretch();
  QFrame* left_frame = new QFrame();
  left_frame->setObjectName("left_frame");
  left_frame->setFixedWidth(480);
  left_frame->setLayout(left_frame_layout);

  list_title3_ = new QLabel(tr("Continue"), this);
  list_title3_->setObjectName("list_title3");
  list_item3_ = new QLabel(
      QString("1.%1\n2.%2")
          .arg(tr("Please make sure all data were made a backup, "
                  "then continue"))
          .arg(tr("Continuing installation will format your disk")),
      this);
  list_item3_->setObjectName("list_item3");
  list_item3_->setWordWrap(true);
  QVBoxLayout* right_frame_layout = new QVBoxLayout();
  right_frame_layout->setContentsMargins(20, 20, 20, 20);
  right_frame_layout->setSpacing(0);
  right_frame_layout->addWidget(list_title3_);
  right_frame_layout->addWidget(list_item3_);
  right_frame_layout->addStretch();
  QFrame* right_frame = new QFrame();
  right_frame->setObjectName("right_frame");
  right_frame->setFixedWidth(480);
  right_frame->setLayout(right_frame_layout);

  reject_button_ = new ExpandedNavButton(tr("Reboot"));
  accept_button_ = new ExpandedNavButton(tr("Continue"));
  cancel_button_ = new NavButton(tr("Back"));

  QGridLayout* content_layout = new QGridLayout();
  content_layout->setContentsMargins(0, 0, 0, 0);
  content_layout->setAlignment(Qt::AlignCenter);
  content_layout->setVerticalSpacing(kMainLayoutSpacing);
  content_layout->setHorizontalSpacing(40);
  content_layout->setColumnMinimumWidth(0, 480);
  content_layout->setColumnMinimumWidth(1, 480);
  content_layout->addWidget(left_frame, 0, 0);
  content_layout->addWidget(right_frame, 0, 1);
  content_layout->addWidget(reject_button_, 1, 0);
  content_layout->addWidget(accept_button_, 1, 1);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(content_layout);
  layout->addStretch();
  layout->addWidget(cancel_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setStyleSheet(ReadFile(":/styles/partition_table_warning_frame.css"));
}

void PartitionTableWarningFrame::onConfirmButtonClicked() {
  emit this->confirmed(device_path_);
}

}  // namespace installer
