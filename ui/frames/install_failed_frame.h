// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_FAILED_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_FAILED_FRAME_H

#include <QFrame>
class QLabel;

namespace ui {

class IconButton;
class NavButton;
class QRWidget;

// Displays error message and a QR image
class InstallFailedFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallFailedFrame(QWidget* parent = nullptr);

  // Reload error message from log file.
  void updateErrorMessage();

 signals:
  // Emitted when reboot button is clocked.
  void finished();

 private:
  void initConnections();
  void initUI();

  NavButton* reboot_button_ = nullptr;
  QRWidget* qr_widget_ = nullptr;
  QLabel* content_label_ = nullptr;
  IconButton* control_button_ = nullptr;

 private slots:
  void onControlButtonClicked();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_FAILED_FRAME_H
