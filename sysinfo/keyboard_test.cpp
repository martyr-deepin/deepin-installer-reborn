// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/keyboard.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(KeyboardTest, ReadXkbConfigTest) {
  XkbConfig config = ReadXkbConfig();
  EXPECT_GT(config.model_list.length(), 10);
  EXPECT_GT(config.layout_list.length(), 100);
}

}  // namespace
}  // namespace installer