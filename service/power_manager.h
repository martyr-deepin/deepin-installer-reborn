// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_POWER_MANAGER_H
#define INSTALLER_SERVICE_POWER_MANAGER_H

namespace installer {

// Reboot system by calling `systemctl reboot`
bool RebootSystem();

// Reboot system by sending magic key to kernel.
bool RebootSystemWithMagicKey();

// Shutdown system by calling `systemctl poweroff` `
bool ShutdownSystem();

// Shutdown system by sending magic key to kernel.
bool ShutdownSystemWithMagicKey();

}  // namespace installer

#endif  // INSTALLER_SERVICE_POWER_MANAGER_H
