// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/users.h"

#include <QString>
#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace sysinfo {
namespace {

TEST(UsersTest, GetSystemUsers) {
  EXPECT_TRUE(GetSystemUsers().contains(QStringLiteral("root")));
}

}  // namespace
}  // namespace sysinfo