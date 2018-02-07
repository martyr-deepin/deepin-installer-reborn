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