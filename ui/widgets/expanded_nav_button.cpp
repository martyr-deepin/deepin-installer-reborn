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

#include "ui/widgets/expanded_nav_button.h"

#include "base/file_util.h"

namespace installer {

ExpandedNavButton::ExpandedNavButton(const QString& text, QWidget* parent)
  : NavButton(text, parent) {
  this->setObjectName("expanded_nav_button");

  // Remove width constraints in NavButton.
  this->setFixedWidth(QWIDGETSIZE_MAX);
  this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

}  // namespace installer