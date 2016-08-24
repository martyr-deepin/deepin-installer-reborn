// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_password.h"

#include "service/settings_manager.h"
#include "service/settings_name.h"

namespace sysinfo {

namespace {

// Check whether chars in |pattern| exists in |str|.
bool ContainsChar(const QString& str, const QString& pattern) {
  for (const QChar& chr : pattern) {
    if (str.contains(chr)) {
      return true;
    }
  }
  return false;
}

}  // namespace

ValidatePasswordState ValidatePassword(const QString& password) {
  const int min_len = service::GetSettingsValue(
      service::kSystemInfoPasswordMinLength).toInt();
  if (password.length() < min_len) {
    return ValidatePasswordState::TooShortError;
  }

  const int max_len = service::GetSettingsValue(
      service::kSystemInfoPasswordMaxLength).toInt();
  if (password.length() > max_len) {
    return ValidatePasswordState::TooLongError;
  }

  const bool need_lower_case = service::GetSettingsBool(
      service::kSystemInfoPasswordShallContainLowerCase);
  if (need_lower_case &&
      (!ContainsChar(password, QStringLiteral("abcdefghijklmnopqrstuvwxyz")))) {
    return ValidatePasswordState::NoLowerCharError;
  }

  const bool need_upper_case = service::GetSettingsBool(
      service::kSystemInfoPasswordShallContainUpperCase);
  if (need_upper_case &&
      (!ContainsChar(password, QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZ")))) {
    return ValidatePasswordState::NoUpperCharError;
  }

  const bool need_special_char = service::GetSettingsBool(
      service::kSystemInfoPasswordShallContainSpecialChar);
  if (need_special_char &&
      (!ContainsChar(password, QStringLiteral("~!@#$%^&*()[]{}\\|/?,.<>")))) {
    return ValidatePasswordState::NoSpecialCharError;
  }

  return ValidatePasswordState::Ok;
}

}  // namespace sysinfo