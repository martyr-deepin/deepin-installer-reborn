// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/partition_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

PartitionFrame::PartitionFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("partition_frame"));

  this->initUI();
  this->initConnections();
}

void PartitionFrame::initConnections() {
  connect(next_button_, &QPushButton::clicked,
          this, &PartitionFrame::onNextButtonClicked);
}

void PartitionFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Select Installation Location"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Please make sure you have backed up important data, then select "
         "the disk to install"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  next_button_ = new NavButton(tr("Start installation"));
  QHBoxLayout* next_layout = new QHBoxLayout();
  next_layout->addWidget(next_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(next_layout);

  this->setLayout(layout);
}

void PartitionFrame::onNextButtonClicked() {
  if (true) {
    emit this->finished();
  }
}

}  // namespace ui