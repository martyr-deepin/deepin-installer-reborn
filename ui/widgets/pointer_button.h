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

#ifndef INSTALLER_POINTER_BUTTON_H
#define INSTALLER_POINTER_BUTTON_H

#include <QPushButton>

namespace installer {

// Change cursor shape when mouse moves over button
class PointerButton : public QPushButton {
  Q_OBJECT

 public:
  explicit PointerButton(QWidget* parent = nullptr);
  PointerButton(const QString& text, QWidget* parent = nullptr);

 protected:
  // Override these two event handlers to implements hover effect.
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;
};

}  // namespace installer

#endif  // INSTALLER_POINTER_BUTTON_H
