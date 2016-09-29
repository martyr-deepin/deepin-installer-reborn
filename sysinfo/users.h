// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_USERS_H
#define INSTALLER_SYSINFO_USERS_H

#include <QStringList>

namespace installer {

const int kNormalUserMinimumUid = 1000;

// Get system username (uid < 1000).
QStringList GetSystemUsers();

// Get all username list in current system.
QStringList GetUsers();

// Check whether current user has root privilege.
bool HasRootPrivilege();

}  // namespace installer

#endif  // INSTALLER_SYSINFO_USERS_H
