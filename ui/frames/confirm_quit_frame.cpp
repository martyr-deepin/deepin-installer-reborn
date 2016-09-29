// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/confirm_quit_frame.h"

#include <QHBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

ConfirmQuitFrame::ConfirmQuitFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("confirm_quit_frame"));

  this->initUI();
}

void ConfirmQuitFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Abort Installation"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Relevant operations you made in the installation process will not "
         "take effect, abort or continue installation?"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  NavButton* continue_button = new NavButton(tr("Continue"));
  QHBoxLayout* continue_layout = new QHBoxLayout();
  continue_layout->addWidget(continue_button);

  NavButton* abort_button = new NavButton(tr("Abort"));
  QHBoxLayout* abort_layout = new QHBoxLayout();
  abort_layout->addWidget(abort_button);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(continue_layout);
  layout->addLayout(abort_layout);

  this->setLayout(layout);

  connect(continue_button, &QPushButton::clicked,
          this, &ConfirmQuitFrame::quitCancelled);
  connect(abort_button, &QPushButton::clicked,
          this, &ConfirmQuitFrame::quitConfirmed);
}

}  // namespace installer