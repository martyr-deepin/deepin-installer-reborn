// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H
#define INSTALLER_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H

#include <QFrame>

namespace installer {

class NavButton;

// Displays a window to select available boot path.
class SelectBootloaderFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SelectBootloaderFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when back-button is clicked.
  void finished();

 private:
  void initConnections();
  void initUI();

  NavButton* back_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H
