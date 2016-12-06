// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/hooks_pack.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(HooksPackTest, HooksPackInitTest) {
  HooksPack before_chroot;
  before_chroot.init(HookType::BeforeChroot, 0, 20, false, nullptr);
  EXPECT_FALSE(before_chroot.hooks.isEmpty());
}

}  // namespace
}  // namespace installer