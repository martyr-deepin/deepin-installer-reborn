// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/system_info_form_frame.h"

#include <QHBoxLayout>
#include <QStringList>
#include <QVBoxLayout>
#include <QtCore/QEvent>

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/validate_hostname.h"
#include "sysinfo/validate_password.h"
#include "sysinfo/validate_username.h"
#include "ui/frames/consts.h"
#include "ui/widgets/avatar_button.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/line_edit.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/system_info_tip.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const char kTextTitle[] = "Create User Account";
const char kTextComment[] = "Input username and password";
const char kTextUsername[] = "Username";
const char kTextHostname[] = "Computer name";
const char kTextPassword[] = "Password";
const char kTextPassword2[] = "Reenter password";
const char kTextNext[] = "Next";

}  // namespace

SystemInfoFormFrame::SystemInfoFormFrame(QWidget* parent)
    : QFrame(parent),
      is_hostname_edited_(false) {
  this->setObjectName("system_info_form_frame");

  this->initUI();
  this->initConnections();
}

void SystemInfoFormFrame::autoConf() {
  WriteUsername(GetSettingsString(kSystemInfoDefaultUsername));
  WriteHostname(GetSettingsString(kSystemInfoDefaultHostname));
  WritePassword(GetSettingsString(kSystemInfoDefaultPassword));
}

void SystemInfoFormFrame::updateAvatar(const QString& avatar) {
  avatar_button_->updateIcon(avatar);
}

void SystemInfoFormFrame::updateTimezone(const QString& timezone) {
  timezone_button_->setText(timezone);
}

void SystemInfoFormFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr(kTextTitle));
    comment_label_->setText(tr(kTextComment));
    username_edit_->setText(tr(kTextUsername));
    hostname_edit_->setText(tr(kTextHostname));
    password_edit_->setText(tr(kTextPassword));
    password2_edit_->setText(tr(kTextPassword2));
    next_button_->setText(tr(kTextNext));
  } else {
    QFrame::changeEvent(event);
  }
}

void SystemInfoFormFrame::initConnections() {
  connect(avatar_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::avatarClicked);
  connect(next_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::onNextButtonClicked);
  connect(timezone_button_, &QPushButton::clicked,
          this, &SystemInfoFormFrame::timezoneClicked);

  connect(username_edit_, &LineEdit::editingFinished,
          this, &SystemInfoFormFrame::onUsernameEditingFinished);
  connect(hostname_edit_, &LineEdit::editingFinished,
          this, &SystemInfoFormFrame::onHostnameEditingFinished);
  connect(password_edit_, &LineEdit::editingFinished,
          this, &SystemInfoFormFrame::onPasswordEditingFinished);
  connect(password2_edit_, &LineEdit::editingFinished,
          this, &SystemInfoFormFrame::onPassword2EditingFinished);

//  connect(username_edit_, &LineEdit::gotFocus,
//          tooltip_, &SystemInfoTip::hide);
//  connect(hostname_edit_, &LineEdit::gotFocus,
//          tooltip_, &SystemInfoTip::hide);
//  connect(password_edit_, &LineEdit::gotFocus,
//          tooltip_, &SystemInfoTip::hide);
//  connect(password2_edit_, &LineEdit::gotFocus,
//          tooltip_, &SystemInfoTip::hide);

  connect(username_edit_, &LineEdit::textEdited,
          this, &SystemInfoFormFrame::onEditingLineEdit);
  connect(username_edit_, &LineEdit::textEdited,
          this, &SystemInfoFormFrame::onUsernameEdited);
  connect(hostname_edit_, &LineEdit::textEdited,
          this, &SystemInfoFormFrame::onEditingLineEdit);
  connect(hostname_edit_, &LineEdit::textEdited,
          this, &SystemInfoFormFrame::onHostnameEdited);
  connect(password_edit_, &LineEdit::textEdited,
          this, &SystemInfoFormFrame::onEditingLineEdit);
  connect(password2_edit_, &LineEdit::textEdited,
          this, &SystemInfoFormFrame::onEditingLineEdit);
}

void SystemInfoFormFrame::initUI() {
  timezone_button_ = new IconButton(":/images/timezone.png",
                                    ":/images/timezone.png",
                                    ":/images/timezone.png",
                                    128, 32, nullptr);
  title_label_ = new TitleLabel(tr(kTextTitle));
  comment_label_ = new CommentLabel(tr(kTextComment));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  avatar_button_ = new AvatarButton();

  username_edit_ = new LineEdit(":/images/username_12.png");
  username_edit_->setPlaceholderText(tr(kTextUsername));
  username_edit_->setText(GetSettingsString(kSystemInfoDefaultUsername));
  username_edit_->setReadOnly(GetSettingsBool(kSystemInfoLockUsername));

  hostname_edit_ = new LineEdit(":/images/hostname_12.png");
  hostname_edit_->setPlaceholderText(tr(kTextHostname));
  hostname_edit_->setText(GetSettingsString(kSystemInfoDefaultHostname));
  hostname_edit_->setReadOnly(GetSettingsBool(kSystemInfoLockHostname));

  password_edit_ = new LineEdit(":/images/password_12.png");
  password_edit_->setPlaceholderText(tr(kTextPassword));
  password_edit_->setEchoMode(QLineEdit::Password);
  password_edit_->setText(GetSettingsString(kSystemInfoDefaultPassword));
  password_edit_->setReadOnly(GetSettingsBool(kSystemInfoLockPassword));

  password2_edit_ = new LineEdit(":/images/password_12.png");
  password2_edit_->setPlaceholderText(tr(kTextPassword2));
  password2_edit_->setEchoMode(QLineEdit::Password);
  password2_edit_->setText(password_edit_->text());
  password2_edit_->setReadOnly(password_edit_->isReadOnly());

  tooltip_ = new SystemInfoTip(this);
  // Same width as line edit.
  tooltip_->setMaximumWidth(550);
  tooltip_->setMinimumHeight(36);
  tooltip_->hide();

  next_button_ = new NavButton(tr(kTextNext));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addWidget(timezone_button_, 0, Qt::AlignLeft);
  layout->addStretch(1);
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
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

bool SystemInfoFormFrame::validateUsername(QString& msg) {
  const ValidateUsernameState state =
      ValidateUsername(username_edit_->text(),
                       GetSettingsInt(kSystemInfoUsernameMinLen),
                       GetSettingsInt(kSystemInfoUsernameMaxLen));
  switch (state) {
    case ValidateUsernameState::ReservedError: {
      msg = tr("Username is already in use");
      return false;
    }
    case ValidateUsernameState::EmptyError: {
      msg = tr("Username is empty!");
      return false;
    }
    case ValidateUsernameState::FirstCharError: {
      msg = tr("First character is invalid");
      return false;
    }
    case ValidateUsernameState::InvalidCharError: {
      msg = tr("Invalid character!");
      return false;
    }
    case ValidateUsernameState::TooLongError: {
      msg = tr("User name has too many characters");
      return false;
    }
    default: {
      return true;
    }
  }
}

bool SystemInfoFormFrame::validateHostname(QString& msg) {
  const ValidateHostnameState state =
      ValidateHostname(hostname_edit_->text(),
                       GetSettingsInt(kSystemInfoHostnameMinLen),
                       GetSettingsInt(kSystemInfoHostnameMaxLen),
                       GetSettingsStringList(kSystemInfoHostnameReserved));
  switch (state) {
    case ValidateHostnameState::EmptyError: {
      msg = tr("Hostname is empty");
      return false;
    }
    case ValidateHostnameState::ReservedError: {
      msg = tr("Hostname is reserved");
      return false;
    }
    case ValidateHostnameState::TooLongError: {
      msg = tr("Hostname is too long");
      return false;
    }
    case ValidateHostnameState::TooShortError: {
      msg = tr("Hostname is too short");
      return false;
    }
    default: {
      return true;
    }
  }
}

bool SystemInfoFormFrame::validatePassword(QString& msg) {
  const ValidatePasswordState state =
      ValidatePassword(password_edit_->text(),
                       GetSettingsInt(kSystemInfoPasswordMinLen),
                       GetSettingsInt(kSystemInfoPasswordMaxLen),
                       GetSettingsBool(kSystemInfoPasswordRequireNumber),
                       GetSettingsBool(kSystemInfoPasswordRequireLowerCase),
                       GetSettingsBool(kSystemInfoPasswordRequireUpperCase),
                       GetSettingsBool(kSystemInfoPasswordRequireSpecialChar));

  switch (state) {
    case ValidatePasswordState::EmptyError: {
      msg = tr("Password is empty");
      return false;
    }
    case ValidatePasswordState::NoLowerCharError: {
      msg = tr("Password does not contain lower case character");
      return false;
    }
    case ValidatePasswordState::NoUpperCharError: {
      msg = tr("Password does not contain upper case character");
      return false;
    }
    case ValidatePasswordState::NoNumberError: {
      msg = tr("Password does not contain number");
      return false;
    }
    case ValidatePasswordState::NoSpecialCharError: {
      msg = tr("Password does not contain special characters");
      return false;
    }
    case ValidatePasswordState::TooShortError: {
      msg = tr("Password too short");
      return false;
    }
    case ValidatePasswordState::TooLongError: {
      msg = tr("Password too long");
      return false;
    }
    default: {
      return true;
    }
  }
}

bool SystemInfoFormFrame::validatePassword2(QString& msg) {
  if (password_edit_->text() != password2_edit_->text()) {
    msg = tr("Password not match");
    return false;
  } else {
    return true;
  }
}

void SystemInfoFormFrame::onNextButtonClicked() {
  QString msg;
  if (!validateUsername(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(username_edit_);
  } else if (!validateHostname(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(hostname_edit_);
  } else if (!validatePassword(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(password_edit_);
  } else if (!validatePassword2(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(password2_edit_);
  } else {
    tooltip_->hide();
    // Write settings to config file when all form inputs are ok.
    WriteUsername(username_edit_->text());
    WriteHostname(hostname_edit_->text());
    WritePassword(password_edit_->text());

    emit this->finished();
  }
}

void SystemInfoFormFrame::onEditingLineEdit() {
  if (tooltip_->isVisible()) {
    tooltip_->hide();
  }
}

void SystemInfoFormFrame::onUsernameEdited() {
  if (!is_hostname_edited_ && !GetSettingsBool(kSystemInfoLockHostname)) {
    const QString username = username_edit_->text();
    if (username.isEmpty()) {
      hostname_edit_->setText("");
    } else {
      // Add suffix to username
      hostname_edit_->setText(
          username + GetSettingsString(kSystemInfoHostnameAutoSuffix));
    }
  }
}

void SystemInfoFormFrame::onUsernameEditingFinished() {
  // When line-edit loses focus, validate it, and check its results.
  // If error occurs, popup tooltip frame.
  QString msg;
  if (!validateUsername(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(username_edit_);
  }
}

void SystemInfoFormFrame::onHostnameEdited() {
  is_hostname_edited_ = true;
}

void SystemInfoFormFrame::onHostnameEditingFinished() {
  QString msg;
  if (!validateHostname(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(hostname_edit_);
  }
}

void SystemInfoFormFrame::onPasswordEditingFinished() {
  QString msg;
  if (!validatePassword(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(password_edit_);
  }
}

void SystemInfoFormFrame::onPassword2EditingFinished() {
  QString msg;
  if (!validatePassword(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(password_edit_);
  } else if (!validatePassword2(msg)) {
    tooltip_->setText(msg);
    tooltip_->showBottom(password2_edit_);
  }
}

}  // namespace installer
