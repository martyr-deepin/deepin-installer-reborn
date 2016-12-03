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
  const ZoneInfo cn("CN", "Asia/Shanghai");
  EXPECT_TRUE(list.contains(cn));
}

TEST(TimezoneTest, GetCurrentTimezone) {
  const QString current_timezone = GetCurrentTimezone();
  EXPECT_FALSE(current_timezone.isEmpty());
  EXPECT_TRUE(IsValidTimezone(current_timezone));
}

}  // namespace
}  // namespace installer