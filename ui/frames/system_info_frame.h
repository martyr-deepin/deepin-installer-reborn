// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_SYSTEM_INFO_FRAME_H
#define INSTALLER_UI_FRAMES_SYSTEM_INFO_FRAME_H

#include <QFrame>
class QStackedLayout;

namespace installer {

class SystemInfoAvatarFrame;
class SystemInfoFormFrame;
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

 public slots:
  // Write conf to file.
  void writeConf();

 private:
  void initConnections();
  void initUI();

  QStackedLayout* stacked_layout_ = nullptr;
  SystemInfoAvatarFrame* avatar_frame_ = nullptr;
  SystemInfoFormFrame* form_frame_ = nullptr;
  SystemInfoTimezoneFrame* timezone_frame_ = nullptr;

  // To mark current page before switching to timezone page.
  QWidget* last_page_ = nullptr;

 private slots:
  // Restore last page when timezone page is finished.
  void restoreLastPage();

  void showAvatarPage();
  void showFormPage();
  void showTimezonePage();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_SYSTEM_INFO_FRAME_H
