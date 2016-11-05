// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/virtual_machine_frame.h"

#include <QVBoxLayout>
#include <QLabel>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

VirtualMachineFrame::VirtualMachineFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("virtual_machine_frame");

  this->initUI();
  this->initConnections();
}

void VirtualMachineFrame::initConnections() {
  connect(next_button_, &QPushButton::clicked,
          this, &VirtualMachineFrame::finished);
}

void VirtualMachineFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Friendly Reminder"));
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("System has detected that you are using a virtual machine, "
         "which will affect the system performance and operation experience, "
         "for a smooth experience, it is recommended to install deepin in "
         "real-machine environment"));
  next_button_ = new NavButton(tr("Continue"));

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(next_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer