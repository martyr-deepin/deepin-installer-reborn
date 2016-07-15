// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H

#include <QFrame>

namespace ui {

class SystemInfoAvatarFrame : public QFrame {
  Q_OBJECT

 public:
  explicit SystemInfoAvatarFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when an avatar is selected.
  void finished();

 private:
  void initConnections();
  void initUI();
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_INNER_SYSTEM_INFO_AVATAR_FORM_H
