// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_FAILED_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_FAILED_FRAME_H

#include <QFrame>

namespace ui {

class NavButton;

class InstallFailedFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallFailedFrame(QWidget* parent = nullptr);

  void updateErrorMessage();

 signals:
  // Emitted when reboot button is clocked.
  void finished();

 private:
  void initConnections();
  void initUI();

  NavButton* reboot_button_ = nullptr;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_FAILED_FRAME_H
