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

#include "ui/widgets/pointer_button.h"

namespace installer {

PointerButton::PointerButton(QWidget* parent) : QPushButton(parent) {
  this->setObjectName("pointer_button");
}

PointerButton::PointerButton(const QString& text, QWidget* parent)
    : PointerButton(parent) {
  this->setText(text);
}

void PointerButton::enterEvent(QEvent* event) {
  this->setCursor(Qt::PointingHandCursor);
  QPushButton::enterEvent(event);
}

void PointerButton::leaveEvent(QEvent* event) {
  this->unsetCursor();
  QPushButton::leaveEvent(event);
}

}  // namespace installer