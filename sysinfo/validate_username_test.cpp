/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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