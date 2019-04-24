/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
  QPixmap m_iconPixmap;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_AVATAR_BUTTON_H
