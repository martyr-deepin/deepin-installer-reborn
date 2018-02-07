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