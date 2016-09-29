// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_hostname.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ValidateHostnameTest, ValidateHostname) {
  EXPECT_FALSE(ValidateHostname(""));
  EXPECT_TRUE(ValidateHostname("domain"));
  EXPECT_TRUE(ValidateHostname("sub.domain"));
  EXPECT_FALSE(ValidateHostname(".sub.domain"));
  EXPECT_FALSE(ValidateHostname("sub-domain."));
  EXPECT_FALSE(ValidateHostname("&:2domain"));
}

TEST(ValidateHostnameTest, ValidateHostnameTemp) {
  EXPECT_TRUE(ValidateHostnameTemp(""));
  EXPECT_TRUE(ValidateHostnameTemp("domain"));
  EXPECT_TRUE(ValidateHostnameTemp("sub.domain"));
  EXPECT_TRUE(ValidateHostnameTemp("sub-domain."));
  EXPECT_FALSE(ValidateHostnameTemp(".sub.domain"));
  EXPECT_FALSE(ValidateHostnameTemp("&:2domain"));
}

}  // namespace
}  // namespace installer