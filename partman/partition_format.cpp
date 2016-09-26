// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_format.h"

#include <QDebug>

#include "base/command.h"

namespace partman {
namespace {

bool FormatBtrfs(const QString& path, const QString& label) {
  // Truncate label size.
  const QString real_label = label.left(255);
  return base::SpawnCmd("mkfs.btrfs",
                        {"-f", QString("-L%1").arg(real_label), path});
}

bool FormatExt2(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return base::SpawnCmd("mkfs.ext2", {"-F", path});
  } else {
    const QString real_label = label.left(16);
    return base::SpawnCmd("mkfs.ext2",
                          {"-F", QString("-L%1").arg(real_label), path});
  }
}

bool FormatExt3(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return base::SpawnCmd("mkfs.ext3", {"-F", path});
  } else {
    const QString real_label = label.left(16);
    return base::SpawnCmd("mkfs.ext3",
                          {"-F", QString("-L%1").arg(real_label), path});
  }
}

bool FormatExt4(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return base::SpawnCmd("mkfs.ext4", {"-F", path});
  } else {
    const QString real_label = label.left(16);
    QString out;
    QString err;
//    return base::SpawnCmd("mkfs.ext4",
//                          {"-F", QString("-L%1").arg(real_label), path});
    bool ok = base::SpawnCmd("mkfs.ext4",
                          {"-F", QString("-L%1").arg(real_label), path}, out, err);
    qDebug() << out;
    qDebug() << err;
    return ok;
  }
}

bool FormatFat16(const QString& path, const QString& label) {
  const QString real_label = label.left(11);
  return base::SpawnCmd("mkfs.msdos",
                        {"-F16", "-v", "-I",
                         QString("-n%1").arg(real_label), path});
}

bool FormatFat32(const QString& path, const QString& label) {
  const QString real_label = label.left(11);
  return base::SpawnCmd("mkfs.msdos",
                        {"-F32", "-v", "-I",
                         QString("-n%1").arg(real_label), path});
}

bool FormatHfs(const QString& path, const QString& label) {
  if (label.isEmpty()) {
    return base::SpawnCmd("hformat", {path});
  } else {
    const QString real_label = label.left(27);
    return base::SpawnCmd("hformat",
                          {QString("-l%1").arg(real_label), path});
  }
}

bool FormatHfsPlus(const QString& path, const QString& label) {
  qDebug() << "hfs plus:" << path << label.length();
  if (label.isEmpty()) {
    return base::SpawnCmd("mkfs.hfsplus", {path});
  } else {
    const QString real_label = label.left(63);
//    return base::SpawnCmd("mkfs.hfsplus",
//                          {QString("-v%1").arg(real_label), path});
    QString out;
    QString err;
    bool ok = base::SpawnCmd("mkfs.hfsplus",
                          {QString("-v%1").arg(real_label), path}, out, err);
    qDebug() << "out:" << out;
    qDebug() << "err:" << err;
    return ok;
  }
}

bool FormatJfs(const QString& path, const QString& label) {
  const QString real_label = label.left(11);
  return base::SpawnCmd("mkfs.jfs",
                        {"-q", QString("-L%1").arg(real_label), path});
}

bool FormatLinuxSwap(const QString& path, const QString& label) {
  const QString real_label = label.left(15);
  return base::SpawnCmd("mkswap", {QString("-L%1").arg(real_label), path});
}

bool FormatNTFS(const QString& path, const QString& label) {
  const QString real_label = label.left(128);
  return base::SpawnCmd("mkntfs",
                        {"-Q", "-v", "-F",
                         QString("-L%1").arg(real_label), path});
}

bool FormatReiser4(const QString& path, const QString& label) {
  const QString real_label = label.left(16);
  return base::SpawnCmd("mkfs.reiser4",
                        {"--force", "--yes",
                         QString("--label%1").arg(real_label), path});
}

bool FormatReiserfs(const QString& path, const QString& label) {
  const QString real_label = label.left(16);
  return base::SpawnCmd("mkreiserfs",
                        {"-f", "-f", QString("--label%1").arg(real_label),
                         path});
}

bool FormatXfs(const QString& path, const QString& label) {
  const QString real_label = label.left(12);
  return base::SpawnCmd("mkfs.xfs",
                        {"-f", QString("-L%1").arg(real_label), path});
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

}  // namespace partman