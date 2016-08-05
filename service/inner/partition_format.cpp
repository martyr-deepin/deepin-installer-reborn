// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/inner/partition_format.h"

#include "base/command.h"

namespace service {

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

}  // namespace service