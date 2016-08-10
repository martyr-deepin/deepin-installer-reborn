// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_PREPARE_INSTALL_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_PREPARE_INSTALL_FRAME_H

#include <QFrame>

namespace ui {

class NavButton;

class PrepareInstallFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PrepareInstallFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when abort-button is clicked, returning to previous page.
  void aborted();

  // Emitted when continue-button is clicked. Start actual installation process.
  void finished();

 private:
  void initConnections();
  void initUI();

  NavButton* abort_button_ = nullptr;
  NavButton* continue_button_ = nullptr;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_PREPARE_INSTALL_FRAME_H
