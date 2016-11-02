// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_form_frame.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/validate_hostname.h"
#include "sysinfo/validate_username.h"
#include "ui/frames/consts.h"
#include "ui/widgets/avatar_button.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/line_edit.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/system_info_tip.h"
#include "ui/widgets/title_label.h"

namespace installer {

SystemInfoFormFrame::SystemInfoFormFrame(QWidget* parent)
    : QFrame(parent),
      is_username_validated_(false),
      is_hostname_validated_(false),
      is_password_validated_(false),
      is_password2_validated_(false) {
  this->setObjectName(QStringLiteral("system_info_form_frame"));

  this->initUI();
  this->initConnections();
}

void SystemInfoFormFrame::updateAvatar(const QString& avatar) {
  avatar_button_->updateIcon(avatar);
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

  connect(username_edit_, &QLineEdit::textChanged,
          this, &SystemInfoFormFrame::onUsernameChanged);
  connect(hostname_edit_, &QLineEdit::textChanged,
          this, &SystemInfoFormFrame::onHostnameChanged);
  connect(password_edit_, &QLineEdit::textChanged,
          this, &SystemInfoFormFrame::onPasswordChanged);
  connect(password2_edit_, &QLineEdit::textChanged,
          this, &SystemInfoFormFrame::onPassword2Changed);
}

void SystemInfoFormFrame::initUI() {
  timezone_button_ = new IconButton(QStringLiteral(":/images/timezone.png"),
                                    QStringLiteral(":/images/timezone.png"),
                                    QStringLiteral(":/images/timezone.png"),
                                    128, 32, nullptr);
  TitleLabel* title_label = new TitleLabel(tr("Create User Account"));
  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("Input username and password"));
  avatar_button_ = new AvatarButton();

  username_edit_ = new LineEdit(QStringLiteral(":/images/username_12.png"));
  username_edit_->setPlaceholderText(tr("Username"));
  hostname_edit_ = new LineEdit(QStringLiteral(":/images/hostname_12.png"));
  hostname_edit_->setPlaceholderText(tr("Computer name"));
  password_edit_ = new LineEdit(QStringLiteral(":/images/password_12.png"));
  password_edit_->setPlaceholderText(tr("Password"));
  password2_edit_ = new LineEdit(QStringLiteral(":/images/password_12.png"));
  password2_edit_->setPlaceholderText(tr("Reenter password"));

  tooltip_ = new SystemInfoTip(this);
  // Same width as line edit.
  tooltip_->setMaximumWidth(550);
  tooltip_->setMinimumHeight(36);
  tooltip_->hide();

  next_button_ = new NavButton(tr("Next"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addWidget(timezone_button_, 0, Qt::AlignLeft);
  layout->addStretch(1);
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch(1);
  layout->addWidget(avatar_button_, 0, Qt::AlignCenter);
  layout->addStretch(1);
  layout->addWidget(username_edit_, 0, Qt::AlignCenter);
  layout->addWidget(hostname_edit_, 0, Qt::AlignCenter);
  layout->addWidget(password_edit_, 0, Qt::AlignCenter);
  layout->addWidget(password2_edit_, 0, Qt::AlignCenter);
  layout->addStretch(3);
  layout->addWidget(next_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

void SystemInfoFormFrame::validateUsername(bool empty_ok) {
  const ValidateUsernameState state = ValidateUsername(username_edit_->text());
  switch (state) {
    case ValidateUsernameState::AlreadyUsedError: {
      username_edit_->setToolTip(tr("Username is already in use"));
      break;
    }
    case ValidateUsernameState::EmptyError: {
      if (!empty_ok) {
        username_edit_->setToolTip(tr("Username is empty!"));
      }
      break;
    }
    case ValidateUsernameState::FirstCharError: {
      username_edit_->setToolTip("First character is invalid");
      break;
    }
    case ValidateUsernameState::InvalidCharError: {
      username_edit_->setToolTip(tr("Invalid character!"));
      break;
    }
    case ValidateUsernameState::Ok: {
      username_edit_->setToolTip("");
      break;
    }
    case ValidateUsernameState::TooLongError: {
      username_edit_->setToolTip(tr("User name has too many characters"));
      break;
    }
  }
  is_username_validated_ = (state == ValidateUsernameState::Ok);
}

void SystemInfoFormFrame::validateHostname(bool empty_ok) {
  if (empty_ok) {
    is_hostname_validated_ = ValidateHostnameTemp(hostname_edit_->text());
  } else {
    is_hostname_validated_ = ValidateHostname(hostname_edit_->text());
  }
  if (!is_hostname_validated_) {
    hostname_edit_->setToolTip(tr("Invalid hostname!"));
  }
}

void SystemInfoFormFrame::validatePassword(bool empty_ok) {
  Q_UNUSED(empty_ok);
  is_password_validated_ = true;
}

void SystemInfoFormFrame::validatePassword2(bool empty_ok) {
  Q_UNUSED(empty_ok);
  is_password2_validated_ = true;
}

void SystemInfoFormFrame::onNextButtonClicked() {
  if (is_username_validated_ && is_hostname_validated_ &&
      is_password_validated_ && is_password2_validated_) {
    WriteUsername(username_edit_->text());
    WriteHostname(hostname_edit_->text());
    WritePassword(password_edit_->text());

    emit this->finished();
  } else {
    if (!is_username_validated_) {
      validateUsername(false);
    }

    if (!is_hostname_validated_) {
      validateHostname(false);
    }

    if (!is_password_validated_) {
      validatePassword(false);
    }
  }
}

void SystemInfoFormFrame::onUsernameChanged() {
  validateUsername(true);
  if (!is_username_validated_ && (!username_edit_->toolTip().isEmpty())) {
    tooltip_->setText(username_edit_->toolTip());
    tooltip_->showBottom(username_edit_);
  } else {
    tooltip_->hide();
  }
}

void SystemInfoFormFrame::onHostnameChanged() {
  validateHostname(true);
}

void SystemInfoFormFrame::onPasswordChanged() {
  validatePassword(true);
}

void SystemInfoFormFrame::onPassword2Changed() {
  validatePassword2(true);
}

}  // namespace installer
