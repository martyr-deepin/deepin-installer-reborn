// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/virtual_machine_frame.h"

#include <QLabel>
#include <QVBoxLayout>

#include "ui/widgets/header_label.h"
#include "ui/widgets/nav_button.h"

namespace ui {

VirtualMachineFrame::VirtualMachineFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("virtual_machine_frame"));

  this->initUI();
}

void VirtualMachineFrame::initUI() {
  HeaderLabel* header_label = new HeaderLabel(tr("Friendly Reminder"));

  NavButton* nav_button = new NavButton(tr("Continue"));

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setAlignment(Qt::AlignCenter);
  layout->addStretch(4);
  layout->addWidget(header_label);
  layout->addStretch(4);
  layout->addWidget(nav_button);
  layout->addStretch(1);

  this->setLayout(layout);

  connect(nav_button, &QPushButton::clicked,
          this, &VirtualMachineFrame::finished);
}

}  // namespace ui