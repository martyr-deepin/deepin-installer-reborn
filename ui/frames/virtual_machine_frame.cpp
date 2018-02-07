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

#include "ui/frames/virtual_machine_frame.h"

#include <QEvent>
#include <QLabel>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

VirtualMachineFrame::VirtualMachineFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("virtual_machine_frame");

  this->initUI();
  this->initConnections();
}

void VirtualMachineFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Friendly Reminder"));
    comment_label_->setText(
        tr("System has detected that you are using a virtual machine, "
           "which will affect the system performance and operation experience, "
           "for a smooth experience, it is recommended to install deepin "
           "in real-machine environment"));
    next_button_->setText(tr("Continue"));
  } else {
    QFrame::changeEvent(event);
  }
}

void VirtualMachineFrame::initConnections() {
  connect(next_button_, &QPushButton::clicked,
          this, &VirtualMachineFrame::finished);
}

void VirtualMachineFrame::initUI() {
  title_label_ = new TitleLabel(tr("Friendly Reminder"));
  comment_label_ = new CommentLabel(
      tr("System has detected that you are using a virtual machine, "
         "which will affect the system performance and operation experience, "
         "for a smooth experience, it is recommended to install deepin "
         "in real-machine environment"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  next_button_ = new NavButton(tr("Continue"));

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(next_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

}  // namespace installer