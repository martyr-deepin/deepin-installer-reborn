// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_format.h"

#include <QDebug>

#include "base/command.h"
#include "partman/fs.h"
#include "partition.h"

namespace partman {
namespace {

bool FormatBtrfs(const QString& path, const QString& label) {
  return base::SpawnCmd("mkfs.btrfs", {QString("-L '%1'").arg(label), path});
}

bool FormatExt2(const QString& path, const QString& label) {
  return base::SpawnCmd("mkfs.ext2",
                        {"-F", QString("-L '%1'").arg(label), path});
}

bool FormatExt3(const QString& path, const QString& label) {
  return base::SpawnCmd("mkfs.ext3",
                        {"-F", QString("-L '%1'").arg(label), path});
}

bool FormatExt4(const QString& path, const QString& label) {
  return base::SpawnCmd("mkfs.ext4",
                        {"-F", QString("-L '%1'").arg(label), path});
}

bool FormatFat16(const QString& path, const QString& label) {
  // Pad fat label with spaces to prevent mlabel writing corrupted labels.
  // see bug #700228 of gparted.
  return base::SpawnCmd("mkfs.msdos",
                        {"-F16", "-v", "-I",
                         QString("-n %1 ").arg(label), path});
}

bool FormatFat32(const QString& path, const QString& label) {
  return base::SpawnCmd("mkfs.msdos",
                        {"-F32", "-v", "-I",
                         QString("-n %1 ").arg(label), path});
}

bool FormatHfs(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return base::SpawnCmd("hformat", {path});
  }
  return base::SpawnCmd("hformat", {QString("-l '%1'").arg(label), path});
}

bool FormatHfsPlus(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return base::SpawnCmd("mkfs.hfsplus", {path});
  }
  return base::SpawnCmd("mkfs.hfsplus", {QString("-v '%1'").arg(label), path});
}

bool FormatJfs(const QString& path, const QString& label) {
  return base::SpawnCmd("mkfs.jfs",
                        {"-q", QString("-L '%1'").arg(label), path});
}

bool FormatLinuxSwap(const QString& path, const QString& label) {
  return base::SpawnCmd("mkswap", {QString("-L '%1'").arg(label), path});
}

bool FormatNTFS(const QString& path, const QString& label) {
  return base::SpawnCmd("mkntfs",
                        {"-Q", "-v", "-F",
                         QString("-L '%1'").arg(label), path});
}

bool FormatReiser4(const QString& path, const QString& label) {
  return base::SpawnCmd("mkfs.reiser4",
                        {"--force", "--yes",
                         QString("--label '%1'").arg(label), path});
}

bool FormatReiserfs(const QString& path, const QString& label) {
  return base::SpawnCmd("mkreiserfs",
                        {"-f", "-f", QString("--label '%1'").arg(label), path});
}

bool FormatXfs(const QString& path, const QString& label) {
  return base::SpawnCmd("mkfs.xfs",
                        {"-f", QString("-L '%1'").arg(label), path});
}

}  // namespace


// Make filesystem on |partition| based on its fs type.
void Mkfs(Partition& partition) {
  switch (partition.fs) {
    case FsType::Btrfs: {
      FormatBtrfs(partition.path, partition.label);
      break;
    }
    case FsType::Ext2: {
      FormatExt2(partition.path, partition.label);
      break;
    }
    case FsType::Ext3: {
      FormatExt3(partition.path, partition.label);
      break;
    }
    case FsType::Ext4: {
      FormatExt4(partition.path, partition.label);
      break;
    }
    case FsType::Fat16: {
      FormatFat16(partition.path, partition.label);
      break;
    }
    case FsType::EFI:
    case FsType::Fat32: {
      FormatFat32(partition.path, partition.label);
      break;
    }
    case FsType::Hfs: {
      FormatHfs(partition.path, partition.label);
      break;
    }
    case FsType::HfsPlus: {
      FormatHfsPlus(partition.path, partition.label);
      break;
    }
    case FsType::Jfs: {
      FormatJfs(partition.path, partition.label);
      break;
    }
    case FsType::LinuxSwap: {
      FormatLinuxSwap(partition.path, partition.label);
      break;
    }
    case FsType::NTFS: {
      FormatNTFS(partition.path, partition.label);
      break;
    }
    case FsType::Reiser4: {
      FormatReiser4(partition.path, partition.label);
      break;
    }
    case FsType::Reiserfs: {
      FormatReiserfs(partition.path, partition.label);
      break;
    }
    case FsType::Xfs: {
      FormatXfs(partition.path, partition.label);
      break;
    }
    default: {
      qWarning() << "Unsupported filesystem to format!" << partition.path;
    }
  }
}

}  // namespace partman