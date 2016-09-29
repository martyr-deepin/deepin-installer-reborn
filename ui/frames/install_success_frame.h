// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INSTALL_SUCCESS_FRAME_H
#define INSTALLER_UI_FRAMES_INSTALL_SUCCESS_FRAME_H

#include <QFrame>

namespace installer {

class NavButton;

class InstallSuccessFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallSuccessFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when reboot button is clicked.
  void finished();

 private:
  void initConnections();
  void initUI();

  NavButton* reboot_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INSTALL_SUCCESS_FRAME_H
