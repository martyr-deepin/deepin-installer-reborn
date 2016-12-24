// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/disk_space_insufficient_frame.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "partman/structs.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/proc_meminfo.h"
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

// Get content of comment label.
// Value of minimum disk space is changed based on size of physical memory.
QString GetCommentLabel() {
  const MemInfo mem_info = GetMemInfo();
  const qint64 mem_threshold =
      GetSettingsInt(kPartitionMemoryThresholdForSwapArea) * kGibiByte;
  int minimum = 0;
  // Check whether memory is too small.
  if (mem_info.mem_total <= mem_threshold) {
    minimum = GetSettingsInt(kPartitionMinimumDiskSpaceRequiredInLowMemory);
  } else {
    minimum = GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
  }
  const int recommended = GetSettingsInt(kPartitionRecommendedDiskSpace);
  return QObject::tr(kTextComment).arg(minimum).arg(recommended);
}

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
    comment_label_->setText(GetCommentLabel());
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
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(GetCommentLabel());
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