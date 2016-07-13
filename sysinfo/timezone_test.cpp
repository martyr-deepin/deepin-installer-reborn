// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/timezone.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace sysinfo {
namespace {

TEST(TimezoneTest, GetZoneInfo) {
  const QStringList timezone_list = GetZoneInfo();
  EXPECT_TRUE(timezone_list.length() > 0);
  EXPECT_TRUE(timezone_list.contains("Asia/Shanghai"));
}

TEST(TimezoneTest, GetCurrentTimezone) {
  const QString current_timezone = GetCurrentTimezone();
  EXPECT_FALSE(current_timezone.isEmpty());
  EXPECT_TRUE(GetZoneInfo().contains(current_timezone));
}

}  // namespace
}  // namespace sysinfo