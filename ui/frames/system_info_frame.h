// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_SYSTEM_INFO_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_SYSTEM_INFO_FRAME_H

#include <QFrame>
class QStackedLayout;

namespace ui {

class SystemInfoAvatarFrame;
class SystemInfoFormFrame;
class SystemInfoTimezoneFrame;

// Provides a form to let use input username, password and hostname,
// select timezone and avatar.
class SystemInfoFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoFrame(QWidget* parent = nullptr);

  void autoConf();

 signals:
  // Emitted when form is validated.
  void finished();

 private:
  void initConnections();
  void initUI();

  QStackedLayout* stacked_layout_ = nullptr;
  SystemInfoAvatarFrame* avatar_frame_ = nullptr;
  SystemInfoFormFrame* form_frame_ = nullptr;
  SystemInfoTimezoneFrame* timezone_frame_ = nullptr;

 private slots:
  void showAvatarPage();
  void showFormPage();
  void showTimezonePage();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_SYSTEM_INFO_FRAME_H
