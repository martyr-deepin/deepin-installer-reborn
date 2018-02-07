/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INSTALLER_PARTMAN_FS_H
#define INSTALLER_PARTMAN_FS_H

#include <QDebug>
#include <QList>
#include <QString>

namespace installer {

enum class FsType {
  Empty,
  Btrfs,
  EFI,
  Ext2,
  Ext3,
  Ext4,
  F2fs,
  Fat16,
  Fat32,
  Hfs,
  HfsPlus,
  Jfs,
  LinuxSwap,
  LVM2PV,
  Nilfs2,
  NTFS,
  Others,
  Reiser4,
  Reiserfs,
  Xfs,
  Unknown,
};
QDebug& operator<<(QDebug& debug, const FsType& fs_type);

// Convert between FsType enum and its name.
FsType GetFsTypeByName(const QString& name);
QString GetFsTypeName(FsType fs_type);

typedef QList<FsType> FsTypeList;

}  // namespace installer

#endif  // INSTALLER_PARTMAN_FS_H
