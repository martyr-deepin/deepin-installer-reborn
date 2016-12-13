// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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