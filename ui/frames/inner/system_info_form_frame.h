// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H

#include <QFrame>

namespace installer {

class AvatarButton;
class IconButton;
class LineEdit;
class NavButton;
class SystemInfoTip;

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

  // Validate line-edit. If failed, write tooltip to |msg| and returns false.
  bool validateUsername(QString& msg);
  bool validateHostname(QString& msg);
  bool validatePassword(QString& msg);
  bool validatePassword2(QString& msg);

  IconButton* timezone_button_ = nullptr;
  AvatarButton* avatar_button_ = nullptr;
  LineEdit* username_edit_ = nullptr;
  LineEdit* hostname_edit_ = nullptr;
  LineEdit* password_edit_ = nullptr;
  LineEdit* password2_edit_ = nullptr;
  // Display tooltip error message.
  SystemInfoTip* tooltip_ = nullptr;
  NavButton* next_button_ = nullptr;

 private slots:
  // Validate form content.
  void onNextButtonClicked();

  // Hide tooltip frame when line-edit is being edited.
  void onEditingLineEdit();
  void onUserNameEditingFinished();
  void onHostnameEditingFinished();
  void onPasswordEditingFinished();
  void onPassword2EditingFinished();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H
