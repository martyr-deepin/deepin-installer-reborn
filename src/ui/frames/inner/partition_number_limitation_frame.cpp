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

#include "ui/frames/inner/partition_number_limitation_frame.h"

#include <QEvent>
#include <QVBoxLayout>

#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

PartitionNumberLimitationFrame::PartitionNumberLimitationFrame(
    QWidget* parent) : QFrame(parent) {
  this->setObjectName("partition_number_limitation_frame");

  this->initUI();
  this->initConnections();
}

void
PartitionNumberLimitationFrame::setHasExtendedPartition(bool has_ext_part) {
  if (has_ext_part) {
    comment1_label_->setText(
        tr("You should delete a primary partition before creating a new one"));
    comment2_label_->setText(
        tr("New logical partitions can only be created near "
           "existing logical partitions"));
    comment2_label_->show();
  } else {
    comment1_label_->setText(
        tr("You should delete a primary partition before creating a new partition"));
    comment2_label_->hide();
  }
}

void PartitionNumberLimitationFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Cannot create new partitions"));
    comment1_label_->setText(
        tr("You should delete a primary partition before creating a new one"));
    comment2_label_->setText(
        tr("New logical partitions can only be created near "
           "existing logical partitions"));
    back_button_->setText(tr("Back"));
  } else {
    QFrame::changeEvent(event);
  }
}

void PartitionNumberLimitationFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &PartitionNumberLimitationFrame::finished);
}

void PartitionNumberLimitationFrame::initUI() {
  title_label_ = new TitleLabel(tr("Cannot create new partitions"));
  comment1_label_ = new CommentLabel(
      tr("You should delete a primary partition before creating a new one"));
  comment2_label_ = new CommentLabel(
      tr("New logical partitions can only be created near "
         "existing logical partitions"));
  back_button_ = new NavButton(tr("Back"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(40);
  layout->addWidget(comment1_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(20);
  layout->addWidget(comment2_label_, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(back_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

}  // namespace installer
