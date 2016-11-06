// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/confirm_quit_frame.h"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "Abort Installation";
const char kTextComment[] = "Relevant operations you made in the installation "
    "process will not take effect, abort or continue installation?";
const char kTextAbort[] = "Abort";
const char kTextContinue[] = "Continue";

}  // namespace

ConfirmQuitFrame::ConfirmQuitFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("confirm_quit_frame");

  this->initUI();
  this->initConnections();
}

void ConfirmQuitFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    qDebug() << "change event";
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
    continue_button_->setText(tr(kTextContinue));
    abort_button_->setText(tr(kTextAbort));
  } else {
    QFrame::changeEvent(event);
  }
}

void ConfirmQuitFrame::initConnections() {
  connect(continue_button_, &QPushButton::clicked,
          this, &ConfirmQuitFrame::quitCancelled);
  connect(abort_button_, &QPushButton::clicked,
          this, &ConfirmQuitFrame::quitConfirmed);
}

void ConfirmQuitFrame::initUI() {
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  abort_button_ = new NavButton(tr(kTextAbort));
  continue_button_ = new NavButton(tr(kTextContinue));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(continue_button_, 0, Qt::AlignCenter);
  layout->addWidget(abort_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer