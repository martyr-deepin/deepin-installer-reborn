/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INSTALLER_PARTMAN_PARTITION_USAGE_H
#define INSTALLER_PARTMAN_PARTITION_USAGE_H

#include <QString>

#include "partman/fs.h"

namespace installer {

// Read partition usage at |partition_path| with type |fs_type|
// Returns false if failed, both |freespace| and |total| are set to -1.
bool ReadUsage(const QString& partition_path,
               FsType fs_type,
               qint64& freespace,
               qint64& total);

}  // namespace installer

#endif  // INSTALLER_PARTMAN_PARTITION_USAGE_H
