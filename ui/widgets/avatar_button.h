// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_WIDGETS_AVATAR_BUTTON_H
#define DEEPIN_INSTALLER_REBORN_UI_WIDGETS_AVATAR_BUTTON_H

#include "ui/widgets/flat_button.h"

class QPaintEvent;

namespace ui {

// Displays user avatar icon.
class AvatarButton : public FlatButton {
  Q_OBJECT

 public:
  AvatarButton(const QString& icon, QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QString icon_;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_WIDGETS_AVATAR_BUTTON_H
