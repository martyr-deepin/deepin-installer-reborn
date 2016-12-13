// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/disk_space_insufficient_frame.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "Insufficient Disk Space";
const char kTextAbort[] = "Abort";

const char kTextComment[] = "It takes at lease %1GB disk space to install "
    "deepin, for better performance, %2GB and more space is recommended";

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
    const int required = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
    const int recommended = GetSettingsInt(kPartitionRecommendedDiskSpace);
    comment_label_->setText(tr(kTextComment).arg(required).arg(recommended));
    abort_button_->setText(tr(kTextAbort));
  } else {
    QFrame::changeEvent(event);
  }
}

void DiskSpaceInsufficientFrame::initConnections() {
  connect(abort_button_, &QPushButton::clicked,
          this, &DiskSpaceInsufficientFrame::finished);
}

void DiskSpaceInsufficientFrame::initUI() {
  const int required = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
  const int recommended = GetSettingsInt(kPartitionRecommendedDiskSpace);
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(
      tr(kTextComment).arg(required).arg( recommended));
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