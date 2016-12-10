// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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