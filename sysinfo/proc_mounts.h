// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SYSINFO_PROC_MOUNTS_H
#define DEEPIN_INSTALLER_REBORN_SYSINFO_PROC_MOUNTS_H

#include <QList>
#include <QString>

namespace sysinfo {

struct MountItem {
  QString path;
  QString mount;  // mount point for the filesystem
  QString fs;  // filesystem type
  QString options;
  // int freq;
  // int passno;
};

typedef QList<MountItem> MountItemList;

// Parse /proc/mounts file.
MountItemList ParseMountItems();

}  // namespace sysinfo

#endif  // DEEPIN_INSTALLER_REBORN_SYSINFO_PROC_MOUNTS_H
