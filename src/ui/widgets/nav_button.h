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

#ifndef INSTALLER_UI_WIDGETS_NAV_BUTTON_H
#define INSTALLER_UI_WIDGETS_NAV_BUTTON_H

#include "ui/widgets/flat_button.h"

namespace installer {

// NavButton is used to switch pages.
// Almost always displays at bottom of frame page.
class NavButton : public FlatButton {
    Q_OBJECT

public:
    NavButton(QWidget *parent = nullptr);
    NavButton(const QString& text, QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_NAV_BUTTON_H
