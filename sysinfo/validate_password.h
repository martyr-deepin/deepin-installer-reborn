// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_VALIDATE_PASSWORD_H
#define INSTALLER_SYSINFO_VALIDATE_PASSWORD_H

#include <QString>

namespace installer {

enum class ValidatePasswordState {
  Ok,
  TooShortError,  // Too few characters in password.
  TooLongError,  // Too many characters in password.
  NoNumberError,  // No number in password.
  NoLowerCharError,  // No lower case characters in password.
  NoUpperCharError,  // No upper case characters in password.
  NoSpecialCharError,  // No special characters in password.
};

// Validate password based on policy below:
//   * min_len: minimum length of |password| allowed;
//   * max_len: maximum length of |password| allowed;
//   * require_number: |password| shall contains number;
//   * require_lower_case: |password| shall contains lower case char;
//   * require_upper_case: |password| shall contains upper case char;
//   * require_special_char: |password| shall contains special char;
ValidatePasswordState ValidatePassword(const QString& password,
                                       int min_len,
                                       int max_len,
                                       bool require_number,
                                       bool require_lower_case,
                                       bool require_upper_case,
                                       bool require_special_char);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_VALIDATE_PASSWORD_H