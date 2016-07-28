// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_mounts.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace sysinfo {
namespace {

TEST(ParseMountItemsTest, ParseMountItems) {
  const QList<MountItem> items = ParseMountItems();
  EXPECT_GT(items.length(), 0);
}

}  // namespace
}  // namespace sysinfo