// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H

#include <QFrame>

namespace installer {

class AvatarButton;
class CommentLabel;
class LineEdit;
class NavButton;
class SystemInfoTip;
class TitleLabel;

class SystemInfoFormFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoFormFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when the avatar button is clicked.
  void avatarClicked();

  // Emitted when form is validated.
  void finished();

 public slots:
  // Update user avatar image.
  void updateAvatar(const QString& avatar);

  // Write form content to conf file.
  void writeConf();

 protected:
  void changeEvent(QEvent* event) override;

  // Set username_edit_ as the default focused widget.
  void showEvent(QShowEvent* event) override;

 private:
  void initConnections();
  void initUI();

  // Validate line-edit. If failed, write tooltip to |msg| and returns false.
  bool validateUsername(QString& msg);
  bool validateHostname(QString& msg);
  bool validatePassword(QString& msg);
  bool validatePassword2(QString& msg);

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  AvatarButton* avatar_button_ = nullptr;
  LineEdit* username_edit_ = nullptr;
  LineEdit* hostname_edit_ = nullptr;
  LineEdit* password_edit_ = nullptr;
  LineEdit* password2_edit_ = nullptr;
  // Display tooltip error message.
  SystemInfoTip* tooltip_ = nullptr;
  NavButton* next_button_ = nullptr;

  // To mark whether content is edited by user.
  bool is_username_edited_;
  bool is_hostname_edited_;
  // If hostname is edited by user, do not generate new hostname based on
  // current username.
  bool is_hostname_edited_manually_;
  bool is_password_edited_;
  bool is_password2_edited_;

 private slots:
  // Validate form content.
  void onNextButtonClicked();

  // Hide tooltip frame when line-edit is being edited.
  void onEditingLineEdit();

  // Automatically change hostname when username is changed by user.
  void onUsernameEdited();
  void onUsernameEditingFinished();
  void onHostnameEdited();
  void onHostnameEditingFinished();
  void onPasswordEdited();
  void onPasswordEditingFinished();
  void onPassword2Edited();
  void onPassword2EditingFinished();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SYSTEM_INFO_FORM_FRAME_H
