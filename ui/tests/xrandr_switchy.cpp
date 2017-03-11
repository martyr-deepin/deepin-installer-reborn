// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/xrandr/xrandr_wrapper.h"

// XRandR Switchy

int main(void) {
//  return (installer::SwitchToMirrorMode() ? 0 : 1);
  return (installer::SwitchMode() ? 0 : 1);
}