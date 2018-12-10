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
                                       bool strong_pwd_check) {
  Q_ASSERT(min_len >= 0);
  Q_ASSERT(max_len > min_len);

  uint success_num = 0;

  if (password.isEmpty() && min_len > 0) {
    return ValidatePasswordState::EmptyError;
  }
  if (password.length() < min_len) {
    return ValidatePasswordState::TooShortError;
  }
  if (password.length() > max_len) {
    return ValidatePasswordState::TooLongError;
  }

  if (ContainsChar(password, "1234567890")) {
    ++success_num;
  }

  if (ContainsChar(password, "abcdefghijklmnopqrstuvwxyz")) {
    ++success_num;
  }

  if (ContainsChar(password, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
    ++success_num;
  }

  if (ContainsChar(password, "~!@#$%^&*()[]{}\\|/?,.<>")) {
    ++success_num;
  }

  if (strong_pwd_check && success_num < 2) {
      return ValidatePasswordState::StrongError;
  }

  return ValidatePasswordState::Ok;
}

}  // namespace installer
