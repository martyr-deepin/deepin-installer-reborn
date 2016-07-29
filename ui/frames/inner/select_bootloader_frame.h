// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H

#include <QFrame>

namespace ui {

class SelectBootloaderFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SelectBootloaderFrame(QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SELECT_BOOTLOADER_FRAME_H
