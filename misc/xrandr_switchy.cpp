// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/utils/xrandr_wrapper.h"

// XRandR Switchy

int main(void) {
  return (installer::SwitchModeWrapper() ? 0 : 1);
}