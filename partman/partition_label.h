// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_LABEL_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_LABEL_H

#include <QString>

#include "partman/fs.h"

namespace partman {

// Read partition label at |partition_path|
QString ReadLabel(const QString& partition_path);

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_LABEL_H
