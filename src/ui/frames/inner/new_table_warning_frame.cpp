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

#include "ui/frames/inner/new_table_warning_frame.h"

#include <QEvent>
#include <QLabel>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/delegates/partition_util.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"
#include "ui/utils/widget_util.h"

namespace installer {

NewTableWarningFrame::NewTableWarningFrame(QWidget* parent)
    : QFrame(parent),
      device_path_() {
  this->setObjectName("new_table_warning_frame");

  this->initUI();
  this->initConnections();
}

QString NewTableWarningFrame::devicePath() const {
  return device_path_;
}

void NewTableWarningFrame::setDevicePath(const QString& device_path,
                                         const QString& device_info) {
  device_path_ = device_path;
  disk_name_label_->setText(device_info);
}

void NewTableWarningFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Full Disk Format Warning"));
    comment_label_->setText(
        tr("Continuing installation will format the whole disk, please make a backup of all your data to avoid data loss"));
    cancel_button_->setText(tr("Cancel"));
    confirm_button_->setText(tr("Continue"));
  } else {
    QFrame::changeEvent(event);
  }
}

void NewTableWarningFrame::initConnections() {
  connect(cancel_button_, &QPushButton::clicked,
          this, &NewTableWarningFrame::canceled);
  connect(confirm_button_, &QPushButton::clicked,
          this, &NewTableWarningFrame::onConfirmButtonClicked);
}

void NewTableWarningFrame::initUI() {
  QLabel* warning_label = new QLabel();
  warning_label->setPixmap(installer::renderPixmap(":/images/warning.svg"));
  title_label_ = new TitleLabel(tr("Full Disk Format Warning"));
  title_label_->setObjectName("title_label");

  QLabel* disk_label = new QLabel();
  disk_label->setPixmap(QPixmap(GetOsTypeLargeIcon(OsType::Empty)));
  disk_name_label_ = new QLabel();
  disk_name_label_->setObjectName("disk_name_label");

  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->setContentsMargins(0, 0, 0, 0);
  title_layout->setSpacing(0);
  title_layout->addStretch();
  title_layout->addWidget(warning_label);
  title_layout->addSpacing(8);
  title_layout->addWidget(title_label_);
  title_layout->addStretch();

  comment_label_ = new CommentLabel(
      tr("Continuing installation will format the whole disk, please make a backup of all your data to avoid data loss"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  cancel_button_ = new NavButton(tr("Cancel"));
  confirm_button_ = new NavButton(tr("Continue"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(disk_label, 0, Qt::AlignHCenter);
  layout->addWidget(disk_name_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(40);
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(cancel_button_, 0, Qt::AlignHCenter);
  layout->addSpacing(kNavButtonVerticalSpacing);
  layout->addWidget(confirm_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
  const QString style(ReadFile(":/styles/new_table_warning_frame.css"));
  this->setStyleSheet(style);
  AppendStyleSheet(title_label_, style);
}

void NewTableWarningFrame::onConfirmButtonClicked() {
  emit this->confirmed(device_path_);
}

}  // namespace installer
