// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_CONFIRM_QUIT_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_CONFIRM_QUIT_FRAME_H

#include <QFrame>

namespace ui {

class ConfirmQuitFrame : public QFrame {
  Q_OBJECT

 public:
  explicit ConfirmQuitFrame(QWidget* parent = nullptr);

 private:
  void initUI();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_CONFIRM_QUIT_FRAME_H
