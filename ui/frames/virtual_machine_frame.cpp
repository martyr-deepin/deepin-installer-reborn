// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/virtual_machine_frame.h"

#include <QHBoxLayout>
#include <QLabel>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

VirtualMachineFrame::VirtualMachineFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("virtual_machine_frame"));

  this->initUI();
}

void VirtualMachineFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Friendly Reminder"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("System has detected that you are using a virtual machine, "
         "which will affect the system performance and operation experience, "
         "for a smooth experience, it is recommended to install deepin in "
         "real-machine environment"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  NavButton* nav_button = new NavButton(tr("Continue"));
  QHBoxLayout* nav_layout = new QHBoxLayout();
  nav_layout->addWidget(nav_button);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setSpacing(kMainLayoutSpacing);
  layout->setAlignment(Qt::AlignCenter);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(nav_layout);

  this->setLayout(layout);

  connect(nav_button, &QPushButton::clicked,
          this, &VirtualMachineFrame::finished);
}

}  // namespace installer