// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_failed_frame.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

InstallFailedFrame::InstallFailedFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("install_failed_frame"));

  this->initUI();
  this->initConnections();
}

void InstallFailedFrame::updateErrorMessage() {

}

void InstallFailedFrame::initConnections() {
  connect(reboot_button_, &QPushButton::clicked,
          this, &InstallFailedFrame::finished);
}

void InstallFailedFrame::initUI() {
  QLabel* status_label = new QLabel();
  status_label->setPixmap(QPixmap(":/images/failed.png"));
  QHBoxLayout* status_layout = new QHBoxLayout();
  status_layout->setAlignment(Qt::AlignCenter);
  status_layout->addWidget(status_label);

  TitleLabel* title_label = new TitleLabel(tr("Installation Failed"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Sorry for the inconvenience, you can photo or scan the 2D code "
         "to send us the error log, so we can better solve the issue."));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  reboot_button_ = new NavButton(tr("Exit installation"));
  QHBoxLayout* reboot_layout = new QHBoxLayout();
  reboot_layout->addWidget(reboot_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(status_layout);
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(reboot_layout);

  this->setLayout(layout);
}

}  // namespace ui