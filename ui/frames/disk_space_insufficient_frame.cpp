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

namespace ui {

DiskSpaceInsufficientFrame::DiskSpaceInsufficientFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName(QStringLiteral("disk_space_insufficient_frame"));

  this->initUI();
}

void DiskSpaceInsufficientFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Insufficient Disk Space"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("It takes at lease 8GB disk space to install deepin, "
         "for better performance, 15GB and more space is recommended"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  NavButton* abort_button = new NavButton(tr("Abort"));
  QHBoxLayout* abort_layout = new QHBoxLayout();
  abort_layout->addWidget(abort_button);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(abort_layout);

  this->setLayout(layout);

  connect(abort_button, &QPushButton::clicked,
          this, &DiskSpaceInsufficientFrame::finished);
}

}  // namespace ui