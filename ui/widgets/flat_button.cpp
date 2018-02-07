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

#include "ui/widgets/flat_button.h"

#include "base/file_util.h"
#include "ui/utils/xutil.h"

namespace installer {

FlatButton::FlatButton(QWidget* parent) : PointerButton(parent) {
  this->setFlat(true);
  this->setStyleSheet(ReadFile(":/styles/flat_button.css"));
}

FlatButton::FlatButton(const QString& text, QWidget* parent)
    : FlatButton(parent) {
  this->setText(text);
}

}  // namespace installer