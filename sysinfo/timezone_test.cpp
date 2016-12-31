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
  EXPECT_EQ(info.latitude, 31.14);
  EXPECT_EQ(info.longitude, 121.28);
}

TEST(TimezoneTest, GetCurrentTimezone) {
  const QString current_timezone = GetCurrentTimezone();
  EXPECT_FALSE(current_timezone.isEmpty());
}

TEST(TimezoneTest, GetTimezoneAliasMap) {
  const TimezoneAliasMap map = GetTimezoneAliasMap();
  EXPECT_GT(map.keys().length(), 0);
  EXPECT_TRUE(map.contains("Asia/Chongqing"));
  EXPECT_EQ(map.value("Asia/Chongqing"), "Asia/Shanghai");
}

TEST(Timezonetest, GetTimezoneOffset) {
  EXPECT_TRUE(GetTimezoneOffset("Asia/Shanghai") == "CST+08");
  EXPECT_TRUE(GetTimezoneOffset("Asia/Pyongyang") == "KST+08:30");
  EXPECT_TRUE(GetTimezoneOffset("America/Lima") == "PET-05");
}

TEST(TimezoneTest, IsValidTimezone) {
  EXPECT_TRUE(IsValidTimezone("Asia/Shanghai"));
  EXPECT_TRUE(IsValidTimezone("Etc/UTC"));
  EXPECT_FALSE(IsValidTimezone("Asia/Beijing"));
}

}  // namespace
}  // namespace installer