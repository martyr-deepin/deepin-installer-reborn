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

#include "ui/widgets/icon_button.h"

#include <QMouseEvent>

namespace installer {

IconButton::IconButton(const QString& normal_icon,
                       const QString& hover_icon,
                       const QString& active_icon,
                       int button_width,
                       int button_height,
                       QWidget* parent)
    : FlatButton(parent),
      normal_icon_(QIcon(normal_icon)),
      hover_icon_(QIcon(hover_icon)),
      active_icon_(QIcon(active_icon)) {
  const QSize size(button_width, button_height);
  this->setFixedSize(size);
  this->setIcon(normal_icon_);
}

void IconButton::enterEvent(QEvent* event) {
  this->setIcon(hover_icon_);
  FlatButton::enterEvent(event);
}

void IconButton::leaveEvent(QEvent* event) {
  this->setIcon(normal_icon_);
  FlatButton::leaveEvent(event);
}

void IconButton::mousePressEvent(QMouseEvent* event) {
  this->setIcon(active_icon_);
  FlatButton::mousePressEvent(event);
}

void IconButton::mouseReleaseEvent(QMouseEvent* event) {
  this->setIcon(hover_icon_);
  FlatButton::mouseReleaseEvent(event);
}

}  // namespace installer