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

#ifndef INSTALLER_SYSINFO_VALIDATE_PASSWORD_H
#define INSTALLER_SYSINFO_VALIDATE_PASSWORD_H

#include <QString>

namespace installer {

enum class ValidatePasswordState {
  Ok,
  EmptyError,  // Password is empty.
  TooShortError,  // Too few characters in password.
  TooLongError,  // Too many characters in password.
  NoNumberError,  // No number in password.
  NoLowerCharError,  // No lower case characters in password.
  NoUpperCharError,  // No upper case characters in password.
  NoSpecialCharError,  // No special characters in password.
  StrongError, // No strong check in password
};

// Validate password based on policy below:
//   * min_len: minimum length of |password| allowed;
//   * max_len: maximum length of |password| allowed;
//   * require_number: |password| shall contains number;
//   * require_lower_case: |password| shall contains lower case char;
//   * require_upper_case: |password| shall contains upper case char;
//   * require_special_char: |password| shall contains special char;
ValidatePasswordState ValidatePassword( const QString& password,
                                        int min_len,
                                        int max_len,
                                        bool strong_pwd_check);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_VALIDATE_PASSWORD_H
