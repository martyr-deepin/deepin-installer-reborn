// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/fs.h"

#include "partman/structs.h"

namespace partman {

FsType GetFsTypeByName(const QString& name) {
  const QString lower = name.toLower();
  if (lower.isEmpty()) return FsType::Empty;
  if (lower == kPartitionFsUnused) return FsType::Empty;
  if (lower == "btrfs") return FsType::Btrfs;
  if (lower == "efi") return FsType::EFI;
  if (lower == "ext2") return FsType::Ext2;
  if (lower == "ext3") return FsType::Ext3;
  if (lower == "ext4") return FsType::Ext4;
  if (lower == "fat16") return FsType::Fat16;
  if (lower == "fat32") return FsType::Fat32;
  if (lower == "hfs") return FsType::Hfs;
  if (lower == "hfs+") return FsType::HfsPlus;
  if (lower == "jfs") return FsType::Jfs;
  if (lower.startsWith("linux-swap")) return FsType::LinuxSwap;
  if (lower == "lvm2pv") return FsType::LVM2PV;
  if (lower == "ntfs") return FsType::NTFS;
  if (lower == "others") return FsType::Others;
  if (lower == "reiser4") return FsType::Reiser4;
  if (lower == "resierfs") return FsType::Reiserfs;
  if (lower == "xfs") return FsType::Xfs;
  return FsType::Unknown;
}

}  // namespace partman