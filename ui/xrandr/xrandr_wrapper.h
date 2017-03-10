// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H
#define INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H

#include <QVector>

namespace installer {

struct ConnectedOutput {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  bool primary = false;  // true if current output is primary one.
};
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

}  // namespace installer

#endif  // INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H
