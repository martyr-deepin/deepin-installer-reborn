// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_success_frame.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QtCore/QEvent>

#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"
#include "ui/frames/consts.h"

namespace installer {

namespace {

const char kTextTitle[] = "Successfully Installed";
const char kTextComment[] =
    "Reboot to enjoy the new experience with deepin, hope you like it!";
const char kTextReboot[] = "Experience now";

}  // namespace

InstallSuccessFrame::InstallSuccessFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("install_success_frame");

  this->initUI();
  this->initConnections();
}

void InstallSuccessFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
    reboot_button_->setText(tr(kTextReboot));
  } else {
    QFrame::changeEvent(event);
  }
}

void InstallSuccessFrame::initConnections() {
  connect(reboot_button_, &QPushButton::clicked,
          this, &InstallSuccessFrame::finished);
}

void InstallSuccessFrame::initUI() {
  QLabel* status_label = new QLabel();
  status_label->setPixmap(QPixmap(":/images/succeed.png"));
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  reboot_button_ = new NavButton(tr(kTextReboot));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(status_label, 0, Qt::AlignCenter);
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(reboot_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

}  // namespace installer