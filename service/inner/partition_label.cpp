// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/inner/partition_label.h"

#include <QProcess>

#include "base/command.h"
#include "base/string_util.h"

namespace service {

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
    return output;
  }
  return QString();
}

QString ReadFat16Label(const QString& path) {
  QString output;
  if (base::SpawnCmd("mlabel", {"-s ::", "-i", path}, output)) {
    return base::RegexpLabel("Volume label is ([^(]*)", output);
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
    return output;
  }
  return QString();
}

QString ReadNTFSLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("ntfslabel", {"--force", path}, output)) {
    return output;
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
    return base::RegexpLabel("^label:[\\t ]*(.*)$", output);
  }
  return QString();
}

QString ReadXfsLabel(const QString& path) {
  QString output;
  if (base::SpawnCmd("xfs_db", {"-r", "-c 'label'", path}, output)) {
    return base::RegexpLabel("^label = \\\"(.*)\\\"", output);
  }
  return QString();
}

}  // namespace service