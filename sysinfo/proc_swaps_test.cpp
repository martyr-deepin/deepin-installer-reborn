// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_swaps.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ProcSwapsTest, ParseSwaps) {
  const SwapItemList items = ParseSwaps();
  if (!items.isEmpty()) {
    EXPECT_GT(items.first().size, 0);
  }
}

}  // namespace
}  // namespace installer