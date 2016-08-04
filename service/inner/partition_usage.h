// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_USAGE_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_USAGE_H

#include <QString>

namespace service {

bool ReadBtrfsUsage(const QString& path, qint64& freespace, qint64& total);
bool ReadExt2Usage(const QString& path, qint64& freespace, qint64& total);
bool ReadFat16Usage(const QString& path, qint64& freespace, qint64& total);
bool ReadJfsUsage(const QString& path, qint64& freespace, qint64& total);
bool ReadNTFSUsage(const QString& path, qint64& freespace, qint64& total);
bool ReadReiser4Usage(const QString& path, qint64& freespace, qint64& total);
bool ReadReiserfsUsage(const QString& path, qint64& freespace, qint64& total);
bool ReadXfsUsage(const QString& path, qint64& freespace, qint64& total);

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_INNER_PARTITION_USAGE_H
