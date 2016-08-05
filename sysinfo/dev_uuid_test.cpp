// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/dev_uuid.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace sysinfo {
namespace {

TEST(DevUUIDTest, ParseUUIDDir) {
  const UUIDItems items = ParseUUIDDir();
  EXPECT_GT(items.size(), 0);
}

}  // namespace
}  // namespace sysinfo