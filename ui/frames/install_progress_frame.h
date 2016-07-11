// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H

#include <QFrame>

namespace ui {

class InstallProgressFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallProgressFrame(QWidget* parent = nullptr);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INSTALL_PROGRESS_FRAME_H
