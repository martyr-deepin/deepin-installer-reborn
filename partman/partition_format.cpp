// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_format.h"

#include <QDebug>

#include "base/command.h"

namespace installer {
namespace {

bool FormatBtrfs(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.btrfs", {"-f", path});
  } else {
    // Truncate label size.
    const QString real_label = label.left(255);
    return SpawnCmd("mkfs.btrfs",
                    {"-f", QString("-L%1").arg(real_label), path});
  }
}

bool FormatExt2(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.ext2", {"-F", path});
  } else {
    const QString real_label = label.left(16);
    return SpawnCmd("mkfs.ext2", {"-F", QString("-L%1").arg(real_label), path});
  }
}

bool FormatExt3(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.ext3", {"-F", path});
  } else {
    const QString real_label = label.left(16);
    return SpawnCmd("mkfs.ext3", {"-F", QString("-L%1").arg(real_label), path});
  }
}

bool FormatExt4(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.ext4", {"-F", path}, output, err);
  } else {
    const QString real_label = label.left(16);
    ok = SpawnCmd("mkfs.ext4", {"-F", QString("-L%1").arg(real_label), path},
                  output, err);
  }
  qDebug() << "output:" << output;
  qDebug() << "err:" << err;
  return ok;
}

bool FormatF2fs(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.f2fs", {path});
  } else {
    const QString real_label = label.left(19);
    return SpawnCmd("mkfs.f2fs", {QString("-l%1").arg(real_label), path});
  }
}

bool FormatFat16(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.msdos", {"-F16", "-v", "-I", path});
  } else {
    const QString real_label = label.left(11);
    return SpawnCmd("mkfs.msdos",
                    {"-F16", "-v", "-I",
                     QString("-n%1").arg(real_label), path});
  }
}

bool FormatFat32(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.msdos", {"-F32", "-v", "-I", path});
  } else {
    const QString real_label = label.left(11);
    return SpawnCmd("mkfs.msdos",
                    {"-F32", "-v", "-I",
                     QString("-n%1").arg(real_label), path});
  }
}

bool FormatHfs(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("hformat", {path});
  } else {
    const QString real_label = label.left(27);
    return SpawnCmd("hformat", {QString("-l%1").arg(real_label), path});
  }
}

bool FormatHfsPlus(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.hfsplus", {path});
  } else {
    const QString real_label = label.left(63);
    return SpawnCmd("mkfs.hfsplus", {QString("-v%1").arg(real_label), path});
  }
}

bool FormatJfs(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.jfs", {"-q", path});
  } else {
    const QString real_label = label.left(11);
    return SpawnCmd("mkfs.jfs", {"-q", QString("-L%1").arg(real_label), path});
  }
}

bool FormatLinuxSwap(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkswap", {path});
  } else {
    const QString real_label = label.left(15);
    return SpawnCmd("mkswap", {QString("-L%1").arg(real_label), path});
  }
}

bool FormatNilfs2(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.nilfs2", {path});
  } else {
    const QString real_label = label.left(1);
    return SpawnCmd("mkfs.nilfs2", {QString("-L%1").arg(real_label), path});
  }
}

bool FormatNTFS(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkntfs", {"-Q", "-v", "-F", path});
  } else {
    const QString real_label = label.left(128);
    return SpawnCmd("mkntfs",
                    {"-Q", "-v", "-F", QString("-L%1").arg(real_label), path});
  }
}

bool FormatReiser4(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.reiser4", {"--force", "--yes", path});
  } else {
    const QString real_label = label.left(16);
    return SpawnCmd("mkfs.reiser4",
                    {"--force", "--yes",
                     QString("--label%1").arg(real_label), path});
  }
}

bool FormatReiserfs(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkreiserfs", {"-f", "-f", path});
  } else {
    const QString real_label = label.left(16);
    return SpawnCmd("mkreiserfs",
                    {"-f", "-f", QString("--label%1").arg(real_label), path});
  }
}

bool FormatXfs(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return SpawnCmd("mkfs.xfs", {"-f", path});
  } else {
    const QString real_label = label.left(12);
    return SpawnCmd("mkfs.xfs", {"-f", QString("-L%1").arg(real_label), path});
  }
}

}  // namespace

// Make filesystem on |partition| based on its fs type.
bool Mkfs(const Partition& partition) {
  switch (partition.fs) {
    case FsType::Btrfs: {
      return FormatBtrfs(partition.path, partition.label);
    }
    case FsType::Ext2: {
      return FormatExt2(partition.path, partition.label);
    }
    case FsType::Ext3: {
      return FormatExt3(partition.path, partition.label);
    }
    case FsType::Ext4: {
      return FormatExt4(partition.path, partition.label);
    }
    case FsType::F2fs: {
      return FormatF2fs(partition.path, partition.label);
    }
    case FsType::Fat16: {
      return FormatFat16(partition.path, partition.label);
    }
    case FsType::EFI:
    case FsType::Fat32: {
      return FormatFat32(partition.path, partition.label);
    }
    case FsType::Hfs: {
      return FormatHfs(partition.path, partition.label);
    }
    case FsType::HfsPlus: {
      return FormatHfsPlus(partition.path, partition.label);
    }
    case FsType::Jfs: {
      return FormatJfs(partition.path, partition.label);
    }
    case FsType::LinuxSwap: {
      return FormatLinuxSwap(partition.path, partition.label);
    }
    case FsType::Nilfs2: {
      return FormatNilfs2(partition.path, partition.label);
    }
    case FsType::NTFS: {
      return FormatNTFS(partition.path, partition.label);
    }
    case FsType::Reiser4: {
      return FormatReiser4(partition.path, partition.label);
    }
    case FsType::Reiserfs: {
      return FormatReiserfs(partition.path, partition.label);
    }
    case FsType::Xfs: {
      return FormatXfs(partition.path, partition.label);
    }
    default: {
      qWarning() << "Unsupported filesystem to format!" << partition.path;
      return false;
    }
  }
}

}  // namespace installer