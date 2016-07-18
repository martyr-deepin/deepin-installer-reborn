// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_form_frame.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/widgets/avatar_button.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

SystemInfoFormFrame::SystemInfoFormFrame(QWidget* parent)
  : QFrame(parent) {
  this->setObjectName(QStringLiteral("system_info_form_frame"));

  this->initUI();
  this->initConnections();
}

void SystemInfoFormFrame::initConnections() {
  connect(avatar_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::avatarClicked);
  connect(next_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::onNextButtonClicked);
}

void SystemInfoFormFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Create User Account"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label =
      new CommentLabel(tr("Input username and password"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  avatar_button_ = new AvatarButton(service::GetDefaultAvatar());
  QHBoxLayout* avatar_layout = new QHBoxLayout();
  avatar_layout->addWidget(avatar_button_);

  QLineEdit* username_edit = new QLineEdit();
  QLineEdit* hostname_edit = new QLineEdit();
  QLineEdit* password_edit = new QLineEdit();
  QLineEdit* password2_edit = new QLineEdit();

  QVBoxLayout* form_layout = new QVBoxLayout();
  form_layout->addWidget(username_edit);
  form_layout->addWidget(hostname_edit);
  form_layout->addWidget(password_edit);
  form_layout->addWidget(password2_edit);

  QHBoxLayout* form_wrapper_layout = new QHBoxLayout();
  form_wrapper_layout->addStretch();
  form_wrapper_layout->addLayout(form_layout);
  form_wrapper_layout->addStretch();

  next_button_ = new NavButton(tr("Next"));
  QHBoxLayout* next_layout = new QHBoxLayout();
  next_layout->addWidget(next_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(avatar_layout);
  layout->addStretch();
  layout->addLayout(form_wrapper_layout);
  layout->addStretch();
  layout->addLayout(next_layout);

  this->setLayout(layout);
}

void SystemInfoFormFrame::onNextButtonClicked() {
  if (true) {
    emit this->finished();
  }
}

}  // namespace ui