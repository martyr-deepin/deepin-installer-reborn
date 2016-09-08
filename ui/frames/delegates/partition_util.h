// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_UTILS_PARTITION_UTIL_H
#define DEEPIN_INSTALLER_REBORN_UI_UTILS_PARTITION_UTIL_H

#include <QtCore/QtGlobal>

#include "partman/fs.h"

namespace ui {

// Get partition name based on |path|.
QString GetPartitionName(const QString& path);

// Get human readable partition usage.
QString GetPartitionUsage(qint64 freespace, qint64 total);

QString GetFsTypeName(partman::FsType fs_type);

// Check whether specific fs type can be mounted by user.
// linux-swap and efi are mounted at fixed position and thus returns false.1
bool SupportMountPoint(partman::FsType fs_type);

// Convert |size| in byte to gigbyte.
int ToGigByte(qint64 size);

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_UTILS_PARTITION_UTIL_H
