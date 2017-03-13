// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_PARTMAN_OS_PROBER_H
#define INSTALLER_PARTMAN_OS_PROBER_H

#include <QString>
#include <QVector>

#include "partman/structs.h"

namespace installer {

struct OsProberItem {
  QString path;  // Partition path, like "/dev/sda1".
  QString distro_name;  // Distribution name, like "Debian".
  QString description;  // Description name, like "Debian sid",
  OsType type;  // Os type, like linux.
};

typedef QVector<OsProberItem> OsProberItems;

// Scan system wide os information with `os-prober`.
OsProberItems GetOsProberItems();

}  // namespace installer

#endif  // INSTALLER_PARTMAN_OS_PROBER_H
