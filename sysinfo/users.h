// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SYSINFO_USERS_H
#define DEEPIN_INSTALLER_REBORN_SYSINFO_USERS_H

#include <QStringList>

namespace sysinfo {

// Get system username (uid < 1000).
QStringList GetSystemUsers();

// Check whether current user has root privilege.
bool HasRootPrivilege();

}  // namespace sysinfo

#endif  // DEEPIN_INSTALLER_REBORN_SYSINFO_USERS_H
