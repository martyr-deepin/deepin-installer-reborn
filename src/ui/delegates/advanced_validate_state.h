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

#ifndef INSTALLER_UI_DELEGATES_ADVANCED_VALIDATE_STATE_H
#define INSTALLER_UI_DELEGATES_ADVANCED_VALIDATE_STATE_H

#include <QList>
#include <QDebug>

namespace installer {

enum class AdvancedValidateState {
  BootFsInvalid,  // Filesystem used for /boot is not in supported fs list.
  BootPartNumberInvalid,  // Partition for /boot is not the first partition.
  BootTooSmall,
  EfiMissing,
  EfiTooSmall,
  RootMissing,
  RootTooSmall,
  Ok,
};
typedef QList <AdvancedValidateState> AdvancedValidateStates;

}  // namespace installer
#endif  // INSTALLER_UI_DELEGATES_ADVANCED_VALIDATE_STATE_H
