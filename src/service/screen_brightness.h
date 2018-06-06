/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#ifndef DEEPIN_INSTALLER_SERVICE_SCREEN_BRIGHTNESS_H
#define DEEPIN_INSTALLER_SERVICE_SCREEN_BRIGHTNESS_H

namespace installer {

/**
 * Set current brightness for all screens
 * @param value 0-100
 */
void SetBrightness(int value);

void IncreaseBrightness();

void DecreaseBrightness();

}  // namespace installer

#endif  // DEEPIN_INSTALLER_SERVICE_SCREEN_BRIGHTNESS_H
