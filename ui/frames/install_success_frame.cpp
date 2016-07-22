// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_success_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"
#include "ui/frames/consts.h"

namespace ui {

InstallSuccessFrame::InstallSuccessFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("install_success_frame"));

  this->initUI();
  this->initConnections();
}

void InstallSuccessFrame::initConnections() {
  connect(reboot_button_, &QPushButton::clicked,
          this, &InstallSuccessFrame::finished);
}

void InstallSuccessFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Successfully Installed"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Reboot to enjoy the new experience with deepin, hope you like it!"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  reboot_button_ = new NavButton(tr("Experience now"));
  QHBoxLayout* reboot_layout = new QHBoxLayout();
  reboot_layout->addWidget(reboot_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(reboot_layout);

  this->setLayout(layout);
}

}  // namespace ui