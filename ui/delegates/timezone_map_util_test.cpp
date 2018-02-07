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

#include "ui/delegates/timezone_map_util.h"

#include <QDebug>
#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(TimzoneMapUtilTest, ConvertLatitideToY) {
  const double point_y = ConvertLatitudeToY(100.0);
  EXPECT_FLOAT_EQ(point_y, 0.29970101);
}

TEST(TimezoneMapUtilTest, ConvertLongitudeToX) {
  const double point_x = ConvertLongitudeToX(100.0);
  EXPECT_FLOAT_EQ(point_x, 0.74444443);
}

TEST(TimezoneMapUtilTest, GetNearestZones) {
  const ZoneInfoList total_zones = GetZoneInfoList();
  const double threshold = 100.0;
  const int x = 630, y = 190, map_width = 800, map_height = 409;
  const ZoneInfoList zones = GetNearestZones(total_zones, threshold, x, y,
                                             map_width, map_height);
  EXPECT_EQ(zones.length(), 1);
  EXPECT_EQ(zones.first().timezone, "Asia/Shanghai");
}

}  // namespace
}  // namespace installer