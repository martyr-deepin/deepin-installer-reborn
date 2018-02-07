/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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