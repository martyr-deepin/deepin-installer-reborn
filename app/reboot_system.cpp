// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// Test reboot/shutdown function.

#include "service/power_manager.h"

int main(void) {
  installer::RebootSystemWithMagicKey();
  return 0;
}