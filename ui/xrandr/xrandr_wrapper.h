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

#ifndef INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H
#define INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H

#include <QDebug>
#include <QVector>

namespace installer {

struct ConnectedOutput {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  bool primary = false;  // true if current output is primary one.
};
QDebug& operator<<(QDebug& debug, const ConnectedOutput& output);
typedef QVector<ConnectedOutput> ConnectedOutputs;

// Get connected |outputs|, returns false if failed.
bool GetConnectedOutputs(ConnectedOutputs& outputs);

// Switch between display modes.
// If only one output is found, use preferred mode;
// Else
//   If all outputs has the same geometry, then they are in mirror mode.
//     Switch to extended mode and set the first output as primary one.
//   Else If last connected output is primary, use mirror mode;
//   Else set next connected output as primary
//   End if.
// End if.
bool SwitchMode();

// Switch to mirror mode if multiple monitors are connected.
bool SwitchToMirrorMode();

// Set output mode to prefer mode.
bool SwitchToPreferMode();

}  // namespace installer

#endif  // INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H
