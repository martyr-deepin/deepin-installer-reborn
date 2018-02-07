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

#include "partman/fs.h"

#include "partman/structs.h"

namespace installer {

namespace {

// Unused file system.
const char kFsUnused[] = "unused";

}  // namespace

QDebug& operator<<(QDebug& debug, const FsType& fs_type) {
  debug << GetFsTypeName(fs_type);
  return debug;
}

FsType GetFsTypeByName(const QString& name) {
  const QString lower = name.toLower();
  if (lower.isEmpty()) return FsType::Empty;
  if (lower == kFsUnused) return FsType::Empty;
  if (lower == "btrfs") return FsType::Btrfs;
  if (lower == "efi") return FsType::EFI;
  if (lower == "ext2") return FsType::Ext2;
  if (lower == "ext3") return FsType::Ext3;
  if (lower == "ext4") return FsType::Ext4;
  if (lower == "f2fs") return FsType::F2fs;
  if (lower == "fat16") return FsType::Fat16;
  if (lower == "fat32") return FsType::Fat32;
  if (lower == "hfs") return FsType::Hfs;
  if (lower == "hfs+") return FsType::HfsPlus;
  if (lower == "jfs") return FsType::Jfs;
  if (lower.startsWith("linux-swap")) return FsType::LinuxSwap;
  if (lower == "lvm2pv") return FsType::LVM2PV;
  if (lower == "nilfs2") return FsType::Nilfs2;
  if (lower == "ntfs") return FsType::NTFS;
  if (lower == "others") return FsType::Others;
  if (lower == "reiser4") return FsType::Reiser4;
  if (lower == "reiserfs") return FsType::Reiserfs;
  if (lower == "xfs") return FsType::Xfs;
  return FsType::Unknown;
}

QString GetFsTypeName(FsType fs_type) {
  switch (fs_type) {
    case FsType::Empty: { return ""; }
    case FsType::Btrfs: { return "btrfs"; }
    case FsType::EFI: { return "efi"; }
    case FsType::Ext2: { return "ext2"; }
    case FsType::Ext3: { return "ext3"; }
    case FsType::Ext4: { return "ext4"; }
    case FsType::F2fs: { return "f2fs"; }
    case FsType::Fat16: { return "fat16"; }
    case FsType::Fat32: { return "fat32"; }
    case FsType::Hfs: { return "hfs"; }
    case FsType::HfsPlus: { return "hfsplus"; }
    case FsType::Jfs: { return "jfs"; }
    case FsType::LinuxSwap: { return "linux-swap"; }
    case FsType::NTFS: { return "ntfs"; }
    case FsType::Nilfs2: { return "nilfs2"; }
    case FsType::LVM2PV: { return "lvm2 pv"; }
    case FsType::Reiser4: { return "reiser4"; }
    case FsType::Reiserfs: { return "reiserfs"; }
    case FsType::Unknown: { return "unknown"; }
    case FsType::Xfs: { return "xfs"; }
    default: { return ""; }
  }
}

}  // namespace installer