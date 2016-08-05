// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SYSINFO_DEV_UUID_H
#define DEEPIN_INSTALLER_REBORN_SYSINFO_DEV_UUID_H

#include <QHash>
#include <QString>

namespace sysinfo {

typedef QHash<QString, QString> UUIDItems;

// Parse /dev/disk/by-uuid folder.
UUIDItems ParseUUIDDir();

}  // namespace sysinfo

#endif  // DEEPIN_INSTALLER_REBORN_SYSINFO_DEV_UUID_H
