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

#ifndef INSTALLER_PARTMAN_OS_PROBER_H
#define INSTALLER_PARTMAN_OS_PROBER_H

#include <QString>
#include <QVector>

#include "partman/structs.h"

namespace installer {

struct OsProberItem {
  QString path;  // Partition path, like "/dev/sda1".
  QString description;  // Description name, like "Debian sid",
  QString distro_name;  // Distribution name, like "Debian".
  OsType type;  // Os type, like linux.
};

typedef QVector<OsProberItem> OsProberItems;

// Scan system wide os information with `os-prober`.
OsProberItems GetOsProberItems();

}  // namespace installer

#endif  // INSTALLER_PARTMAN_OS_PROBER_H
