// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_username.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ValidateUsernameTest, ValidateUsername) {
  const QString reserved_username_file = RESOURCES_DIR "/reserved_usernames";
  EXPECT_EQ(ValidateUsername("root", reserved_username_file, 1, 10),
            ValidateUsernameState::ReservedError);
  EXPECT_EQ(ValidateUsername("", reserved_username_file, 1, 10),
            ValidateUsernameState::EmptyError);
  EXPECT_EQ(ValidateUsername("AbcUser", reserved_username_file, 1, 10),
            ValidateUsernameState::FirstCharError);
  EXPECT_EQ(ValidateUsername("a:invalid", reserved_username_file, 1, 10),
            ValidateUsernameState::InvalidCharError);
  EXPECT_EQ(ValidateUsername("hello_world", reserved_username_file, 1, 5),
            ValidateUsernameState::TooLongError);
  EXPECT_EQ(ValidateUsername("hello", reserved_username_file, 6, 10),
            ValidateUsernameState::TooShortError);
  EXPECT_EQ(ValidateUsername("not-used-username", reserved_username_file,
                             1, 24),
            ValidateUsernameState::Ok);
}

}  // namespace
}  // namespace installer