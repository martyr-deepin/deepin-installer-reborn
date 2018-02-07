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

#ifndef INSTALLER_SYSINFO_DEV_DISK_H
#define INSTALLER_SYSINFO_DEV_DISK_H

#include <QHash>
#include <QString>

namespace installer {

typedef QHash<QString, QString> LabelItems;
typedef QHash<QString, QString> PartLabelItems;
typedef QHash<QString, QString> UUIDItems;

// Parse /dev/disk/by-label folder.
LabelItems ParseLabelDir();

// Parse /dev/disk/by-partlabel folder.
PartLabelItems ParsePartLabelDir();

// Parse /dev/disk/by-uuid folder.
UUIDItems ParseUUIDDir();

}  // namespace installer

#endif  // INSTALLER_SYSINFO_DEV_DISK_H
