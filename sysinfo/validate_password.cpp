// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_password.h"

namespace installer {

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

ValidatePasswordState ValidatePassword(const QString& password,
                                       int min_len,
                                       int max_len,
                                       bool require_number,
                                       bool require_lower_case,
                                       bool require_upper_case,
                                       bool require_special_char) {
  Q_ASSERT(min_len >= 0);
  Q_ASSERT(max_len > min_len);

  if (password.isEmpty() && min_len > 0) {
    return ValidatePasswordState::EmptyError;
  }
  if (password.length() < min_len) {
    return ValidatePasswordState::TooShortError;
  }
  if (password.length() > max_len) {
    return ValidatePasswordState::TooLongError;
  }

  if (require_number && (!ContainsChar(password, QStringLiteral("1234567890")))) {
    return ValidatePasswordState::NoNumberError;
  }

  if (require_lower_case &&
      (!ContainsChar(password, QStringLiteral("abcdefghijklmnopqrstuvwxyz")))) {
    return ValidatePasswordState::NoLowerCharError;
  }

  if (require_upper_case &&
      (!ContainsChar(password, QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZ")))) {
    return ValidatePasswordState::NoUpperCharError;
  }

  if (require_special_char &&
      (!ContainsChar(password, QStringLiteral("~!@#$%^&*()[]{}\\|/?,.<>")))) {
    return ValidatePasswordState::NoSpecialCharError;
  }

  return ValidatePasswordState::Ok;
}

}  // namespace installer