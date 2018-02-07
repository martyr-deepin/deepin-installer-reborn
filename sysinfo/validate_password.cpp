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

  if (require_number && (!ContainsChar(password, "1234567890"))) {
    return ValidatePasswordState::NoNumberError;
  }

  if (require_lower_case &&
      (!ContainsChar(password, "abcdefghijklmnopqrstuvwxyz"))) {
    return ValidatePasswordState::NoLowerCharError;
  }

  if (require_upper_case &&
      (!ContainsChar(password, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"))) {
    return ValidatePasswordState::NoUpperCharError;
  }

  if (require_special_char &&
      (!ContainsChar(password, "~!@#$%^&*()[]{}\\|/?,.<>"))) {
    return ValidatePasswordState::NoSpecialCharError;
  }

  return ValidatePasswordState::Ok;
}

}  // namespace installer