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

ConfirmQuitFrame::ConfirmQuitFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("confirm_quit_frame");

  this->initUI();
  this->initConnections();
}

void ConfirmQuitFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Abort Installation"));
    comment_label_->setText(
        tr("Relevant operations you made in the installation "
           "process will not take effect, abort or continue installation?"));
    continue_button_->setText(tr("Continue"));
    abort_button_->setText(tr("Abort"));
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
  title_label_ = new TitleLabel(tr("Abort Installation"));
  comment_label_ = new CommentLabel(
      tr("Relevant operations you made in the installation "
         "process will not take effect, abort or continue installation?"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  continue_button_ = new NavButton(tr("Continue"));
  abort_button_ = new NavButton(tr("Abort"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(continue_button_, 0, Qt::AlignCenter);
  layout->addSpacing(15);
  layout->addWidget(abort_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer