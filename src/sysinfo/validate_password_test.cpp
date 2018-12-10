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

#include "sysinfo/validate_password.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ValidatePasswordTest, ValidatePassword) {
  EXPECT_EQ(ValidatePassword("", 5, 10, false),
            ValidatePasswordState::EmptyError);
  EXPECT_EQ(ValidatePassword("ABC", 8, 12, false),
            ValidatePasswordState::TooShortError);

  EXPECT_EQ(ValidatePassword("ABCDE12345", 4, 8, false),
            ValidatePasswordState::TooLongError);

  EXPECT_EQ(ValidatePassword("abced", 4, 12, true),
            ValidatePasswordState::StrongError);

  EXPECT_EQ(ValidatePassword("ABCED", 4, 12, true),
            ValidatePasswordState::StrongError);

  EXPECT_EQ(ValidatePassword("12345", 4, 12, true),
            ValidatePasswordState::StrongError);

}

}  // namespace
}  // namespace installer
