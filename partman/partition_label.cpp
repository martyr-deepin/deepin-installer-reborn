// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition_label.h"

#include <QDebug>

#include "base/command.h"
#include "base/string_util.h"

namespace partman {

namespace {

// To store `blkid` output.
QString g_blkid_cache;

QString GetBlkidEntryByPath(const QString& path) {
  if (g_blkid_cache.isEmpty()) {
    if (!base::SpawnCmd("blkid", {}, g_blkid_cache)) {
      qWarning() << "ReadLabel() call `blkid` failed!";
      return "";
    }
  }
  const QString pattern(QString("^%1:([^\\n]+)").arg(path));
  return base::RegexpLabel(pattern, g_blkid_cache);
}

}  // namespace

QString ReadLabel(const QString& path) {
  const QString blkid_entry = GetBlkidEntryByPath(path);
  if (!blkid_entry.isEmpty()) {
    const QString pattern("LABEL=\"([^\"]*)\"");
    return base::RegexpLabel(pattern, blkid_entry);
  } else {
    return "";
  }
}

}  // namespace partman