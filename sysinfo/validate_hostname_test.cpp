// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_hostname.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ValidateHostnameTest, ValidateHostname) {
  EXPECT_EQ(ValidateHostname("", {}), ValidateHostnameState::EmptyError);
  EXPECT_EQ(ValidateHostname("-domain", {}),
            ValidateHostnameState::InvalidChar);
  EXPECT_EQ(ValidateHostname("sub.domain", {}),
            ValidateHostnameState::Ok);
  EXPECT_EQ(ValidateHostname("localhost", {"localhost", "loop"}),
            ValidateHostnameState::ReservedError);
  EXPECT_EQ(ValidateHostname("sub-domain", {}),
            ValidateHostnameState::Ok);
  EXPECT_EQ(ValidateHostname("&:2domain", {}),
            ValidateHostnameState::InvalidChar);
}

}  // namespace
}  // namespace installer