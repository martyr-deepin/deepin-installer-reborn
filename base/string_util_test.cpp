// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/string_util.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(StringUtilTest, RegexpLabel) {
  const QString uuid = RegexpLabel("UUID=([^ ]+)", "UUID=1234-321 ");
  EXPECT_EQ(uuid, "1234-321");

  const QString blocks = RegexpLabel("Free blocks:\\s*(\\d+)",
                                     "Free blocks:              5609441");
  EXPECT_EQ(blocks, "5609441");
}

}  // namespace
}  // namespace installer