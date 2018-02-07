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

#ifndef INSTALLER_UI_DELEGATES_INSTALL_SLIDE_FRAME_UTIL_H
#define INSTALLER_UI_DELEGATES_INSTALL_SLIDE_FRAME_UTIL_H

#include <QStringList>

namespace installer {

// Get absolute path to slide folder, based on |locale|.
// |locale| might be empty or like "zh_CN" or "en_US".
QString GetSlideDir(const QString& locale);

// Get slide image file list with specific |locale|.
QStringList GetSlideFiles(const QString& locale);

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_INSTALL_SLIDE_FRAME_UTIL_H