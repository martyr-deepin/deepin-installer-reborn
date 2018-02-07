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

#ifndef INSTALLER_UI_WIDGETS_ICON_BUTTON_H
#define INSTALLER_UI_WIDGETS_ICON_BUTTON_H

#include "ui/widgets/flat_button.h"

#include <QIcon>
class QMouseEvent;

namespace installer {

// IconButton displays different icons in a flat button at different states.
class IconButton : public FlatButton {
  Q_OBJECT

 public:
  IconButton(const QString& normal_icon,
             const QString& hover_icon,
             const QString& active_icon,
             int button_width,
             int button_height,
             QWidget* parent = nullptr);

 protected:
  // Override these methods to change icons based on mouse action.
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  QIcon normal_icon_;
  QIcon hover_icon_;
  QIcon active_icon_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_ICON_BUTTON_H
