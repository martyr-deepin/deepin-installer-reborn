// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H

#include <QFrame>

namespace ui {

class AvatarButton;
class IconButton;
class LineEdit;
class NavButton;

class SystemInfoFormFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoFormFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when the avatar button is clicked.
  void avatarClicked();

  // Emitted when form is validated.
  void finished();

  // Emitted when timezone button is clicked.
  void timezoneClicked();

 public slots:
  // Update user avatar image.
  void updateAvatar(const QString& avatar);

  // Update text in timezone button.
  void updateTimezone(const QString& timezone);

 private:
  void initConnections();
  void initUI();

  void validateUsername(bool empty_ok);
  void validateHostname(bool empty_ok);
  void validatePassword(bool empty_ok);
  void validatePassword2(bool empty_ok);

  IconButton* timezone_button_ = nullptr;
  AvatarButton* avatar_button_ = nullptr;
  LineEdit* username_edit_ = nullptr;
  LineEdit* hostname_edit_ = nullptr;
  LineEdit* password_edit_ = nullptr;
  LineEdit* password2_edit_ = nullptr;
  NavButton* next_button_ = nullptr;

  bool is_username_validated_;
  bool is_hostname_validated_;
  bool is_password_validated_;
  bool is_password2_validated_;

 private slots:
  // Validate form content.
  void onNextButtonClicked();

  void onUsernameChanged();
  void onHostnameChanged();
  void onPasswordChanged();
  void onPassword2Changed();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H
