// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_FS_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_FS_H

#include <QList>
#include <QString>

namespace partman {

enum class FsType {
  Empty,
  Btrfs,
  EFI,
  Ext2,
  Ext3,
  Ext4,
  Fat16,
  Fat32,
  Hfs,
  HfsPlus,
  Jfs,
  LinuxSwap,
  LVM2PV,
  NTFS,
  Others,
  Reiser4,
  Reiserfs,
  Xfs,
  Unknown,
};

FsType GetFsTypeByName(const QString& name);

typedef QList<FsType> FsTypeList;

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_FS_H
