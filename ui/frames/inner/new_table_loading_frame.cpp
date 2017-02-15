// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/new_table_loading_frame.h"

#include <QEvent>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/spinner_label.h"
#include "ui/widgets/title_label.h"

namespace installer {

NewTableLoadingFrame::NewTableLoadingFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("new_table_loading_frame");

  this->initUI();
}

void NewTableLoadingFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Formatting disk"));
    comment_label_->setText(tr("Formatting disks, please wait..."));
  } else {
    QFrame::changeEvent(event);
  }
}

void NewTableLoadingFrame::hideEvent(QHideEvent* event) {
  spinner_label_->stop();
  QFrame::hideEvent(event);
}

void NewTableLoadingFrame::showEvent(QShowEvent* event) {
  spinner_label_->start();
  QFrame::showEvent(event);
}

void NewTableLoadingFrame::initUI() {
  title_label_ = new TitleLabel(tr("Scan disks"));
  comment_label_ = new CommentLabel(tr("Scanning disks, please wait..."));
  spinner_label_ = new SpinnerLabel();

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addWidget(comment_label_, 0, Qt::AlignCenter);
  layout->addSpacing(15);
  layout->addWidget(spinner_label_, 0, Qt::AlignCenter);
  layout->addStretch();

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

}  // namespace installer