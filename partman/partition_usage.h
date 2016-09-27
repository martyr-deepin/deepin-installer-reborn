// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_USAGE_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_USAGE_H

#include <QString>

#include "partman/fs.h"

namespace partman {

// Read partition usage at |partition_path| with type |fs_type|
// Returns false if failed, both |freespace| and |total| are set to -1.
bool ReadUsage(const QString& partition_path,
               FsType fs_type,
               qint64& freespace,
               qint64& total);

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_USAGE_H
