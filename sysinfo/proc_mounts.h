// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_PROC_MOUNTS_H
#define INSTALLER_SYSINFO_PROC_MOUNTS_H

#include <QList>
#include <QString>

namespace installer {

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

}  // namespace installer

#endif  // INSTALLER_SYSINFO_PROC_MOUNTS_H
