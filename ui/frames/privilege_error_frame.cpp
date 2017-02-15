// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/privilege_error_frame.h"

#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

PrivilegeErrorFrame::PrivilegeErrorFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("privilege_error_frame");

  this->initUI();
  this->initConnection();
}

void PrivilegeErrorFrame::initConnection() {
  connect(continue_button_, &QPushButton::clicked,
          this, &PrivilegeErrorFrame::finished);
}

void PrivilegeErrorFrame::initUI() {
  TitleLabel* title_label = new TitleLabel("Privilege error!");
  CommentLabel* comment_label = new CommentLabel(
      "Please execute with root account");

  continue_button_ = new NavButton("Continue");

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignHCenter);
  layout->addWidget(comment_label, 0, Qt::AlignHCenter);
  layout->addStretch();
  layout->addWidget(continue_button_, 0, Qt::AlignHCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

}  // namespace installer