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
class SystemInfoTimezoneFrame;

// Provides a form to let use input username, password and hostname,
// select timezone and avatar.
class SystemInfoFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when form is validated.
  void finished();

  // Called when timezone is updated in SelectLanguageFrame.
  void timezoneUpdated(const QString& timezone);

 public slots:
  // Read default conf from settings.
  void readConf();

  // Notify timezone_frame_ to scan wifi hot spot.
  void scanTimezone();

  // Write conf to file.
  void writeConf();

 private:
  void initConnections();
  void initUI();

  // Update visibility of buttons in header bar based on current page.
  void updateHeadBar();

  QPushButton* timezone_button_ = nullptr;
  QPushButton* keyboard_button_ = nullptr;
  QHBoxLayout* head_layout_ = nullptr;
  QStackedLayout* stacked_layout_ = nullptr;
  SystemInfoAvatarFrame* avatar_frame_ = nullptr;
  SystemInfoFormFrame* form_frame_ = nullptr;
  SystemInfoKeyboardFrame* keyboard_frame_ = nullptr;
  SystemInfoTimezoneFrame* timezone_frame_ = nullptr;

  // To mark current page before switching to timezone page.
  int last_page_;

  // Do not show keyboard frame if this flag is true.
  bool disable_keyboard_;

  // Do not show timezone frame if this flag is true.
  bool disable_timezone_;

 private slots:
  // Restore last page when timezone page is finished.
  void restoreLastPage();

  void showAvatarPage();
  void showFormPage();
  void showKeyboardPage();
  void showTimezonePage();

  // Hide timezone Frame.
  void hideTimezone();

  // Update text in keyboard button.
  void updateLayout(const QString& layout);

  // Update text in timezone button.
  void updateTimezone(const QString& timezone);
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_SYSTEM_INFO_FRAME_H
