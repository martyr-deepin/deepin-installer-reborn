// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_form_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/widgets/avatar_button.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/line_edit.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace ui {

SystemInfoFormFrame::SystemInfoFormFrame(QWidget* parent)
  : QFrame(parent) {
  this->setObjectName(QStringLiteral("system_info_form_frame"));

  this->initUI();
  this->initConnections();
}

void SystemInfoFormFrame::updateTimezone(const QString& timezone) {
  timezone_button_->setText(timezone);
}

void SystemInfoFormFrame::initConnections() {
  connect(avatar_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::avatarClicked);
  connect(next_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::onNextButtonClicked);
  connect(timezone_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::timezoneClicked);
}

void SystemInfoFormFrame::initUI() {
  timezone_button_ = new IconButton(QStringLiteral(":/images/timezone.png"),
                                    QStringLiteral(":/images/timezone.png"),
                                    QStringLiteral(":/images/timezone.png"),
                                    128, 32, nullptr);
  // TODO(xushaohua): Remove timezone text.
  timezone_button_->setText("Beijing");
  QHBoxLayout* timezone_layout = new QHBoxLayout();
  timezone_layout->setAlignment(Qt::AlignLeft);
  timezone_layout->addWidget(timezone_button_);

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

  LineEdit* username_edit =
      new LineEdit(QStringLiteral(":/images/username.png"));
  username_edit->setPlaceholderText(tr("Username"));
  LineEdit* hostname_edit =
      new LineEdit(QStringLiteral(":/images/hostname.png"));
  hostname_edit->setPlaceholderText(tr("Computer name"));
  LineEdit* password_edit =
      new LineEdit(QStringLiteral(":/images/password.png"));
  password_edit->setPlaceholderText(tr("Password"));
  LineEdit* password2_edit =
      new LineEdit(QStringLiteral(":/images/password.png"));
  password2_edit->setPlaceholderText(tr("Reenter password"));

  QVBoxLayout* form_layout = new QVBoxLayout();
  form_layout->setContentsMargins(0, 0, 0, 0);
  form_layout->setMargin(0);
  form_layout->setSpacing(20);
  form_layout->addWidget(username_edit);
  form_layout->addWidget(hostname_edit);
  form_layout->addWidget(password_edit);
  form_layout->addWidget(password2_edit);

  QFrame* form_wrapper = new QFrame();
  form_wrapper->setFixedWidth(400);
  form_wrapper->setLayout(form_layout);
  QHBoxLayout* form_wrapper_layout = new QHBoxLayout();
  form_wrapper_layout->setContentsMargins(0, 0, 0, 0);
  form_wrapper_layout->setSpacing(0);
  form_wrapper_layout->addStretch();
  form_wrapper_layout->addWidget(form_wrapper);
  form_wrapper_layout->addStretch();

  next_button_ = new NavButton(tr("Next"));
  QHBoxLayout* next_layout = new QHBoxLayout();
  next_layout->addWidget(next_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addLayout(timezone_layout);
  layout->addStretch(1);
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch(1);
  layout->addLayout(avatar_layout);
  layout->addStretch(1);
  layout->addLayout(form_wrapper_layout);
  layout->addStretch(3);
  layout->addLayout(next_layout);

  this->setLayout(layout);
}

void SystemInfoFormFrame::onNextButtonClicked() {
  if (true) {
    emit this->finished();
  }
}

}  // namespace ui