// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/disk_space_insufficient_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

DiskSpaceInsufficientFrame::DiskSpaceInsufficientFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName("disk_space_insufficient_frame");

  this->initUI();
}

void DiskSpaceInsufficientFrame::setSizes(int required_device_size,
                                          int recommended_device_size) {
  comment_label_->setText(
      tr("It takes at lease %1GB disk space to install deepin, "
         "for better performance, %2GB and more space is recommended")
          .arg(required_device_size)
          .arg(recommended_device_size));
}

void DiskSpaceInsufficientFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Insufficient Disk Space"));
  comment_label_ = new CommentLabel("");
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addSpacing(0);
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->addWidget(comment_label_);
  NavButton* abort_button = new NavButton(tr("Abort"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(abort_button, 0, Qt::AlignCenter);

  this->setLayout(layout);

  connect(abort_button, &QPushButton::clicked,
          this, &DiskSpaceInsufficientFrame::finished);
}

}  // namespace installer