// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/partition_loading_frame.h"

#include <QEvent>
#include <QHBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "Scanning disks..";
const char kTextComment[] =
    "It may take a few seconds to scan disk devices...";

}  // namespace

PartitionLoadingFrame::PartitionLoadingFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("partition_loading_frame");

  this->initUI();
}

void PartitionLoadingFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
  } else {
    QFrame::changeEvent(event);
  }
}

void PartitionLoadingFrame::initUI() {
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addWidget(comment_label_, 0, Qt::AlignCenter);
  layout->addStretch();

  this->setLayout(layout);
}

}  // namespace installer