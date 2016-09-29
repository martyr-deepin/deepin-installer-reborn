// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_DEV_DISK_H
#define INSTALLER_SYSINFO_DEV_DISK_H

#include <QHash>
#include <QString>

namespace installer {

typedef QHash<QString, QString> LabelItems;
typedef QHash<QString, QString> PartLabelItems;
typedef QHash<QString, QString> UUIDItems;

// Parse /dev/disk/by-label folder.
LabelItems ParseLabelDir();

// Parse /dev/disk/by-partlabel folder.
PartLabelItems ParsePartLabelDir();

// Parse /dev/disk/by-uuid folder.
UUIDItems ParseUUIDDir();

}  // namespace installer

#endif  // INSTALLER_SYSINFO_DEV_DISK_H
