// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/kernel_list.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(KernelListTest, GetKernelList) {
  const KernelList list = GetKernelList();

  EXPECT_FALSE(list.title.isEmpty());
  EXPECT_GT(list.items.length(), 0);
}

}  // namespace
}  // namespace installer