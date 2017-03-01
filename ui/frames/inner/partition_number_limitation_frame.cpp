// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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

void PartitionNumberLimitationFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Cannot create new partitions"));
    comment1_label_->setText(
        tr("You need to delete a primary partition to create "
           "a new primary partition"));
    comment2_label_->setText(
        tr("New logical partitions can only be created near "
           "existing logical partitions"));
    back_button_->setText(tr("Back"));
  } else {
    QFrame::changeEvent(event);
  }}

void PartitionNumberLimitationFrame::initConnections() {
  connect(back_button_, &QPushButton::clicked,
          this, &PartitionNumberLimitationFrame::finished);
}

void PartitionNumberLimitationFrame::initUI() {
  title_label_ = new TitleLabel(tr("Cannot create new partitions"));
  comment1_label_ = new CommentLabel(
      tr("You need to delete a primary partition to create "
         "a new primary partition"));
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