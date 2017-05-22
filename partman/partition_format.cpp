// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_format.h"

#include <QDebug>

#include "base/command.h"
#include "sysinfo/machine.h"

namespace installer {
namespace {

bool FormatBtrfs(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.btrfs", {"-f", path}, output, err);
  } else {
    // Truncate label size.
    const QString real_label = label.left(255);
    ok = SpawnCmd("mkfs.btrfs", {"-f", QString("-L%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatBtrfs() error:" << err;
  }
  return ok;
}

bool FormatExt2(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.ext2", {"-F", path}, output, err);
  } else {
    const QString real_label = label.left(16);
    ok = SpawnCmd("mkfs.ext2", {"-F", QString("-L%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatExt2() err:" << err;
  }
  return ok;
}

bool FormatExt3(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.ext3", {"-F", path}, output, err);
  } else {
    const QString real_label = label.left(16);
    ok = SpawnCmd("mkfs.ext3", {"-F", QString("-L%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatExt3() err:" << err;
  }
  return ok;
}

bool FormatExt4(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  const MachineArch arch = GetMachineArch();
  if (arch == MachineArch::LOONGSON) {
    // Disable 64bit support on loongson platform.
    if (label.isEmpty()) {
      ok = SpawnCmd("mkfs.ext4", {"-O ^64bit", "-F", path}, output, err);
    } else {
      const QString real_label = label.left(16);
      const QString label_opt(QString("-L%1").arg(real_label));
      ok = SpawnCmd("mkfs.ext4", {"-O ^64bit", "-F", label_opt, path},
                    output, err);
    }
  } else {
    if (label.isEmpty()) {
      ok = SpawnCmd("mkfs.ext4", {"-F", path}, output, err);
    } else {
      const QString real_label = label.left(16);
      ok = SpawnCmd("mkfs.ext4", {"-F", QString("-L%1").arg(real_label), path},
                    output, err);
    }
  }

  if (!ok) {
    qCritical() << "FormatExt4() err:" << err;
  }
  return ok;
}

bool FormatF2fs(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.f2fs", {path}, output, err);
  } else {
    const QString real_label = label.left(19);
    ok = SpawnCmd("mkfs.f2fs", {QString("-l%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatF2fs() err:" << err;
  }
  return ok;
}

bool FormatFat16(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.msdos", {"-F16", "-v", "-I", path}, output, err);
  } else {
    const QString real_label = label.left(11);
    ok = SpawnCmd("mkfs.msdos",
                  {"-F16", "-v", "-I", QString("-n%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatFat16() err:" << err;
  }
  return ok;
}

bool FormatFat32(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.msdos", {"-F32", "-v", "-I", path}, output, err);
  } else {
    const QString real_label = label.left(11);
    ok = SpawnCmd("mkfs.msdos",
                  {"-F32", "-v", "-I", QString("-n%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatFat32() err:" << err;
  }
  return ok;
}

bool FormatHfs(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("hformat", {path}, output, err);
  } else {
    const QString real_label = label.left(27);
    ok = SpawnCmd("hformat", {QString("-l%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatHfs() err:" << err;
  }
  return ok;
}

bool FormatHfsPlus(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.hfsplus", {path}, output, err);
  } else {
    const QString real_label = label.left(63);
    ok = SpawnCmd("mkfs.hfsplus", {QString("-v%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatHfsPlus() err:" << err;
  }
  return ok;
}

bool FormatJfs(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.jfs", {"-q", path}, output, err);
  } else {
    const QString real_label = label.left(11);
    ok = SpawnCmd("mkfs.jfs", {"-q", QString("-L%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatJfs() err:" << err;
  }
  return ok;
}

bool FormatLinuxSwap(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkswap", {path}, output, err);
  } else {
    const QString real_label = label.left(15);
    ok = SpawnCmd("mkswap", {QString("-L%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatLinuxSwap() err:" << err;
  }
  return ok;
}

bool FormatNilfs2(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.nilfs2", {path}, output, err);
  } else {
    const QString real_label = label.left(1);
    ok = SpawnCmd("mkfs.nilfs2", {QString("-L%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatNilfs2() err:" << err;
  }
  return ok;
}

bool FormatNTFS(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkntfs", {"-Q", "-v", "-F", path}, output, err);
  } else {
    const QString real_label = label.left(128);
    ok = SpawnCmd("mkntfs",
                  {"-Q", "-v", "-F", QString("-L%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatNTFS() err:" << err;
  }
  return ok;
}

bool FormatReiser4(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.reiser4", {"--force", "--yes", path}, output, err);
  } else {
    const QString real_label = label.left(16);
    ok = SpawnCmd("mkfs.reiser4",
                 {"--force", "--yes",
                  QString("--label%1").arg(real_label), path},
                 output, err);
  }
  if (!ok) {
    qCritical() << "FormatReiser4() err:" << err;
  }
  return ok;
}

bool FormatReiserfs(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkreiserfs", {"-f", "-f", path}, output, err);
  } else {
    const QString real_label = label.left(16);
    ok = SpawnCmd("mkreiserfs",
                  {"-f", "-f", QString("--label%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatReiserfs() err:" << err;
  }
  return ok;
}

bool FormatXfs(const QString& path, const QString& label) {
  QString output;
  QString err;
  bool ok;
  if (label.isEmpty()) {
    ok = SpawnCmd("mkfs.xfs", {"-f", path}, output, err);
  } else {
    const QString real_label = label.left(12);
    ok = SpawnCmd("mkfs.xfs", {"-f", QString("-L%1").arg(real_label), path},
                  output, err);
  }
  if (!ok) {
    qCritical() << "FormatXfs() err:" << err;
  }
  return ok;
}

}  // namespace

// Make filesystem on |partition| based on its fs type.
bool Mkfs(const Partition& partition) {
  qDebug() << "Mkfs()" << partition;
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