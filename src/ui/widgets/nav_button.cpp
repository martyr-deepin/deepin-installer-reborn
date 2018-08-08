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

#include "ui/widgets/nav_button.h"

#include "base/file_util.h"

namespace installer {

NavButton::NavButton(QWidget *parent)
    : NavButton("", parent)
{

}

NavButton::NavButton(const QString& text, QWidget* parent)
  : FlatButton(text, parent) {
  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  this->setStyleSheet(ReadFile(":/styles/nav_button.css"));
  this->setFixedSize(310, 36);
}

}  // namespace installer
