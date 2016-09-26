// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_label.h"

#include <QDebug>
#include <QProcess>

#include "base/command.h"
#include "base/string_util.h"

namespace partman {

namespace {

QString ReadBtrfsLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("btrfs", {"filesystem", "show", path}, output)) {
    return base::RegexpLabel("^Label: '(.*)'  uuid:", output);
  }
  return QString();
}

QString ReadExt2Label(const QString& path) {
  QString output;
  if (base::SpawnCmd("e2label", {path}, output)) {
    return output.trimmed();
  }
  return QString();
}

QString ReadFat16Label(const QString& path) {
  QString output;
  if (base::SpawnCmd("dosfslabel", {path}, output)) {
    return output.trimmed();
  }
  return QString();
}

QString ReadHfsLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("vol_id", {path}, output)) {
    const QString label = base::RegexpLabel("ID_FS_LABEL=([^\\n]*)", output);
    if (label != "untitled") {
      return label;
    }
  }
  return QString();
}

QString ReadJfsLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("jfs_tune", {"-l", path}, output)) {
    return base::RegexpLabel("Volume label:\\s*'(.*)'$", output);
  }
  return QString();
}

QString ReadLinuxSwapLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("swaplabel", {path}, output)) {
    if (!output.isEmpty()) {
      return base::RegexpLabel("LABEL:\\s*([^\\n]*)", output);
    }
  }
  return QString();
}

QString ReadNilfs2Label(const QString& path) {
  QString output;
  if (base::SpawnCmd("nilfs-tune", {"-l", path}, output)) {
    const QString label =
        base::RegexpLabel("^Filesystem volume name:\\s*(.*)$", output);
    if (label != "(none)") {
      return label;
    }
  }
  return QString();
}

QString ReadNTFSLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("ntfslabel", {"--force", path}, output)) {
    return output.trimmed();
  }
  return QString();
}

QString ReadReiser4Label(const QString& path) {
  QString output;
  if (base::SpawnCmd("debugfs.reiser4", {path}, output)) {
    const QString label = base::RegexpLabel("^label:\\s*(.*)$", output);
    if (label != "<none>") {
      return label;
    }
  }
  return QString();
}

QString ReadReiserfsLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("debugreiserfs", {path}, output)) {
    return base::RegexpLabel("^LABEL:\\s*(.*)$", output);
  }
  return QString();
}

QString ReadXfsLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("xfs_db", {"-r", "-c", "label", path}, output)) {
    return base::RegexpLabel("label = \"(.*)\"", output);
  }
  return QString();
}

}  // namespace

QString ReadLabel(const QString& path, FsType fs_type) {
  QString label;
  switch (fs_type) {
    case FsType::Btrfs: {
      label = ReadBtrfsLabel(path);
      break;
    }
    case FsType::Ext2:
    case FsType::Ext3:
    case FsType::Ext4: {
      label = ReadExt2Label(path);
      break;
    }
    case FsType::EFI:
    case FsType::Fat16:
    case FsType::Fat32: {
      label = ReadFat16Label(path);
      break;
    }
    case FsType::Hfs:
    case FsType::HfsPlus: {
      label = ReadHfsLabel(path);
      break;
    }
    case FsType::Jfs: {
      label = ReadJfsLabel(path);
      break;
    }
    case FsType::LinuxSwap: {
      label = ReadLinuxSwapLabel(path);
      break;
    }
    case FsType::Nilfs2: {
      label = ReadNilfs2Label(path);
      break;
    }
    case FsType::NTFS: {
      label = ReadNTFSLabel(path);
      break;
    }
    case FsType::Reiser4: {
      label = ReadReiser4Label(path);
      break;
    }
    case FsType::Reiserfs: {
      label = ReadReiserfsLabel(path);
      break;
    }
    case FsType::Xfs: {
      label = ReadXfsLabel(path);
      break;
    }
    default: {
      label = "";
      break;
    }
  }
  return label;
}

}  // namespace partman