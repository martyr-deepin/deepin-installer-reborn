// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_hostname.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ValidateHostnameTest, ValidateHostname) {
  EXPECT_EQ(ValidateHostname("", 2, 10, {}), ValidateHostnameState::EmptyError);
  EXPECT_EQ(ValidateHostname("domain", 8, 10, {}),
            ValidateHostnameState::TooShortError);
  EXPECT_EQ(ValidateHostname("sub.domain", 2, 6, {}),
            ValidateHostnameState::TooLongError);
  EXPECT_EQ(ValidateHostname("localhost", 2, 10, {"localhost", "loop"}),
            ValidateHostnameState::ReservedError);
  EXPECT_EQ(ValidateHostname("sub-domain", 2, 10, {}),
            ValidateHostnameState::Ok);
  EXPECT_EQ(ValidateHostname("&:2domain", 2, 10, {}),
            ValidateHostnameState::InvalidChar);
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