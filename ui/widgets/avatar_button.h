// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_AVATAR_BUTTON_H
#define INSTALLER_UI_WIDGETS_AVATAR_BUTTON_H

#include "ui/widgets/flat_button.h"

class QPaintEvent;

namespace installer {

// Displays user avatar icon.
class AvatarButton : public FlatButton {
  Q_OBJECT

 public:
  explicit AvatarButton(QWidget* parent = nullptr);
  AvatarButton(const QString& icon, QWidget* parent = nullptr);

  void updateIcon(const QString& icon);
  QString avatar() const { return icon_; }

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QString icon_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_AVATAR_BUTTON_H
