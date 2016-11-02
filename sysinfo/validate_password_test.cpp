// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_password.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ValidatePasswordTest, ValidatePassword) {
  EXPECT_EQ(ValidatePassword("", 5, 10, false, false, false, false),
            ValidatePasswordState::EmptyError);
  EXPECT_EQ(ValidatePassword("ABC", 8, 12, false, false, false, false),
            ValidatePasswordState::TooShortError);

  EXPECT_EQ(ValidatePassword("ABCDE12345", 4, 8, false, false, false, false),
            ValidatePasswordState::TooLongError);

  EXPECT_EQ(ValidatePassword("ABCDEabcde", 4, 12, true, false, false, false),
            ValidatePasswordState::NoNumberError);

  EXPECT_EQ(ValidatePassword("ABCD1234", 4, 12, true, true, false, false),
            ValidatePasswordState::NoLowerCharError);

  EXPECT_EQ(ValidatePassword("abcd1234", 4, 12, true, true, true, false),
            ValidatePasswordState::NoUpperCharError);

  EXPECT_EQ(ValidatePassword("ABCabc1234", 4, 12, true, true, true, true),
            ValidatePasswordState::NoSpecialCharError);
}

}  // namespace
}  // namespace installer