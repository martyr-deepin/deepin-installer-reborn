// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/file_util.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(FileUtil, CopyFodlerTest) {
  EXPECT_TRUE(CopyFolder("/etc/init.d", "/tmp/init.d"));
  EXPECT_TRUE(CopyFolder("/etc/apt", "/tmp/apt", false));
}

}  // namespace
}  // namespace installer