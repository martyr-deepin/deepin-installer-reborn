// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_username.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ValidateUsernameTest, ValidateUsername) {
  EXPECT_EQ(ValidateUsername("root"), ValidateUsernameState::AlreadyUsedError);
  EXPECT_EQ(ValidateUsername(""), ValidateUsernameState::EmptyError);
  EXPECT_EQ(ValidateUsername("AbcUser"), ValidateUsernameState::FirstCharError);
  EXPECT_EQ(ValidateUsername("a:invalid-username"),
            ValidateUsernameState::InvalidCharError);
  EXPECT_EQ(ValidateUsername("not-used-username"), ValidateUsernameState::Ok);
}

}  // namespace
}  // namespace installer