// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/confirm_quit_frame.h"

#include <QHBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

ConfirmQuitFrame::ConfirmQuitFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("confirm_quit_frame");

  this->initUI();
}

void ConfirmQuitFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Abort Installation"));
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("Relevant operations you made in the installation process will not "
         "take effect, abort or continue installation?"));
  NavButton* continue_button = new NavButton(tr("Continue"));
  NavButton* abort_button = new NavButton(tr("Abort"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(continue_button, 0, Qt::AlignCenter);
  layout->addWidget(abort_button, 0, Qt::AlignCenter);

  this->setLayout(layout);

  connect(continue_button, &QPushButton::clicked,
          this, &ConfirmQuitFrame::quitCancelled);
  connect(abort_button, &QPushButton::clicked,
          this, &ConfirmQuitFrame::quitConfirmed);

//  this->setStyleSheet("background-color: red;");
}

}  // namespace installer