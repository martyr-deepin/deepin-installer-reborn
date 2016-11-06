// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/disk_space_insufficient_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtCore/QEvent>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "Insufficient Disk Space";
const char kTextAbort[] = "Abort";

}  // namespace

DiskSpaceInsufficientFrame::DiskSpaceInsufficientFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName("disk_space_insufficient_frame");

  this->initUI();
  this->initConnections();
}

void DiskSpaceInsufficientFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    abort_button_->setText(tr(kTextAbort));
  } else {
    QFrame::changeEvent(event);
  }
}

void DiskSpaceInsufficientFrame::setSizes(int required_device_size,
                                          int recommended_device_size) {
  comment_label_->setText(
      tr("It takes at lease %1GB disk space to install deepin, "
         "for better performance, %2GB and more space is recommended")
          .arg(required_device_size)
          .arg(recommended_device_size));
}

void DiskSpaceInsufficientFrame::initConnections() {
  connect(abort_button_, &QPushButton::clicked,
          this, &DiskSpaceInsufficientFrame::finished);
}

void DiskSpaceInsufficientFrame::initUI() {
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel("");
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->addSpacing(0);
  comment_layout->addWidget(comment_label_);

  abort_button_ = new NavButton(tr(kTextAbort));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(abort_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer