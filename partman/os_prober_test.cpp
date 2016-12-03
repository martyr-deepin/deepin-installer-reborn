// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/os_prober.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace partman {
namespace {

TEST(TestOsProber, GetOsTypeItems) {
  const OsTypeItems os_type_items = GetOsTypeItems();
  EXPECT_FALSE(os_type_items.isEmpty());
  EXPECT_TRUE(os_type_items.value("/dev/sdc1") == OsType::Linux);
}

}  // namespace
}  // namespace partman