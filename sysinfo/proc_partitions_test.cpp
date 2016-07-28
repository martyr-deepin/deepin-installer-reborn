// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_partitions.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace sysinfo {
namespace {

TEST(ParsePartitionItemsTest, ParsePartitionItems) {
  const QList<PartitionItem> items = ParsePartitionItems();
  EXPECT_GT(items.length(), 0);
}

}  // namespace
}  // namespace sysinfo