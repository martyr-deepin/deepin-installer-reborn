// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_success_frame.h"

#include <QHBoxLayout>
#include <QLabel>

#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"
#include "ui/frames/consts.h"

namespace installer {

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
  QLabel* status_label = new QLabel();
  status_label->setPixmap(QPixmap(":/images/succeed.png"));
  TitleLabel* title_label = new TitleLabel(tr("Successfully Installed"));
  CommentLabel* comment_label = new CommentLabel(
      tr("Reboot to enjoy the new experience with deepin, hope you like it!"));
  reboot_button_ = new NavButton(tr("Experience now"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(status_label, 0, Qt::AlignCenter);
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addWidget(comment_label, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(reboot_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer