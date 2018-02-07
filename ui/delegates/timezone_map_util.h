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

#ifndef INSTALLER_DELEGATES_TIMEZONE_MAP_UTIL_H
#define INSTALLER_DELEGATES_TIMEZONE_MAP_UTIL_H

#include "sysinfo/timezone.h"

namespace installer {

// Convert position of zone from polar coordinates to rectangular coordinates.
double ConvertLatitudeToY(double latitude);
double ConvertLongitudeToX(double longitude);

// Get a list of zone info whose distance to (x, y) is less than |threshold|
// in a world map with size (map_width, map_height).
ZoneInfoList GetNearestZones(const ZoneInfoList& total_zones, double threshold,
                             int x, int y, int map_width, int map_height);

}  // namespace installer

#endif  // INSTALLER_DELEGATES_TIMEZONE_MAP_UTIL_H
