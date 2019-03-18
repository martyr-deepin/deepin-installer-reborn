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

static const QMap<installer::FsType, QString> FS_TYPE_MAP{
    { installer::FsType::Empty, "" },
    { installer::FsType::Btrfs, "btrfs" },
    { installer::FsType::EFI, "efi" },
    { installer::FsType::Ext2, "ext2" },
    { installer::FsType::Ext3, "ext3" },
    { installer::FsType::Ext4, "ext4" },
    { installer::FsType::F2fs, "f2fs" },
    { installer::FsType::Fat16, "fat16" },
    { installer::FsType::Fat32, "fat32" },
    { installer::FsType::Hfs, "hfs" },
    { installer::FsType::HfsPlus, "hfsplus" },
    { installer::FsType::Jfs, "jfs" },
    { installer::FsType::NTFS, "ntfs" },
    { installer::FsType::Nilfs2, "nilfs2" },
    { installer::FsType::LVM2PV, "lvm2 pv" },
    { installer::FsType::Reiser4, "reiser4" },
    { installer::FsType::Reiserfs, "reiserfs" },
    { installer::FsType::Unknown, "unknown" },
    { installer::FsType::Xfs, "xfs" }
};

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

    if (lower.isEmpty() || lower == kFsUnused) return FsType::Empty;
    if (lower.startsWith("linux-swap")) return FsType::LinuxSwap;

    return FS_TYPE_MAP.key(lower, FsType::Unknown);
}

QString GetFsTypeName(FsType fs_type) {
    switch (fs_type) {
        case FsType::LinuxSwap: {
            return "linux-swap";
        }
        default: {
            return FS_TYPE_MAP.value(fs_type);
        }
    }
}

}  // namespace installer
