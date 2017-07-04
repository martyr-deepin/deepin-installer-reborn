// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_SYSTEM_INFO_FRAME_H
#define INSTALLER_UI_FRAMES_SYSTEM_INFO_FRAME_H

#include <QFrame>
class QHBoxLayout;
class QPushButton;
class QStackedLayout;

namespace installer {

class SystemInfoAvatarFrame;
class SystemInfoFormFrame;
class SystemInfoKeyboardFrame;

// Provides a form to let use input username, password and hostname,
// select timezone and avatar.
class SystemInfoFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when form is validated.
  void finished();

 public slots:
  // Read default conf from settings.
  void readConf();

  // Write conf to file.
  void writeConf();

 private:
  void initConnections();
  void initUI();

  // Update visibility of buttons in header bar based on current page.
  void updateHeadBar();

  QPushButton* keyboard_button_ = nullptr;
  QHBoxLayout* head_layout_ = nullptr;
  QStackedLayout* stacked_layout_ = nullptr;
  SystemInfoAvatarFrame* avatar_frame_ = nullptr;
  SystemInfoFormFrame* form_frame_ = nullptr;
  SystemInfoKeyboardFrame* keyboard_frame_ = nullptr;

  // To mark current page before switching to timezone page.
  int last_page_;

  // Do not show keyboard frame if this flag is true.
  bool disable_keyboard_;

 private slots:
  // Restore last page when timezone page is finished.
  void restoreLastPage();

  void showAvatarPage();
  void showFormPage();
  void showKeyboardPage();

  // Update text in keyboard button.
  void updateLayout(const QString& layout);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_SYSTEM_INFO_FRAME_H
