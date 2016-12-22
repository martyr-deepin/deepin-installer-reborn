// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_meminfo.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ProcMemInfoTest, GetMemInfoTest) {
  const MemInfo info = GetMemInfo();
  EXPECT_GT(info.mem_total, 0);
  EXPECT_GT(info.buffers, 0);
}

}  // namespace
}  // namespace installer