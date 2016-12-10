// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/timezone.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(TimezoneTest, GetZoneInfoList) {
  const ZoneInfoList list = GetZoneInfoList();
  EXPECT_FALSE(list.isEmpty());
  const int index = GetZoneInfoByZone(list, "Asia/Shanghai");
  EXPECT_TRUE(index > -1);
  const ZoneInfo info = list.at(index);
  EXPECT_EQ(info.latitude, 3114);
  EXPECT_EQ(info.longitude, 12128);
}

TEST(TimezoneTest, GetCurrentTimezone) {
  const QString current_timezone = GetCurrentTimezone();
  EXPECT_FALSE(current_timezone.isEmpty());
  EXPECT_TRUE(IsValidTimezone(current_timezone));
}

}  // namespace
}  // namespace installer