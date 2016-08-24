// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_password.h"

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

namespace sysinfo {
namespace {

TEST(ValidatePasswordTest, ValidatePassword) {
  const bool need_number = service::GetSettingsBool(
      service::kSystemInfoPasswordShallContainNumber);
  if (need_number) {
    EXPECT_EQ(ValidatePassword("ABC"), ValidatePasswordState::NoNumberError);
  }

  const bool need_lower_case = service::GetSettingsBool(
      service::kSystemInfoPasswordShallContainLowerCase);
  if (need_lower_case) {
    EXPECT_EQ(ValidatePassword("ABC123"),
              ValidatePasswordState::NoLowerCharError);
  }

  const bool need_upper_case = service::GetSettingsBool(
      service::kSystemInfoPasswordShallContainUpperCase);
  if (need_upper_case) {
    EXPECT_EQ(ValidatePassword("abc123"),
              ValidatePasswordState::NoUpperCharError);
  }

  const bool need_special_char = service::GetSettingsBool(
      service::kSystemInfoPasswordShallContainSpecialChar);
  if (need_special_char) {
    EXPECT_EQ(ValidatePassword("Abcabc123"),
              ValidatePasswordState::NoSpecialCharError);
  }
}

}  // namespace
}  // namespace sysinfo