// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SYSINFO_VALIDATE_PASSWORD_H
#define DEEPIN_INSTALLER_REBORN_SYSINFO_VALIDATE_PASSWORD_H

#include <QString>

namespace sysinfo {

enum class ValidatePasswordState {
  Ok,
  TooShortError,  // Too few characters in password.
  TooLongError,  // Too many characters in password.
  NoNumberError,  // No number in password.
  NoLowerCharError,  // No lower case characters in password.
  NoUpperCharError,  // No upper case characters in password.
  NoSpecialCharError,  // No special characters in password.
};

// Validate password based on policy defined in settings.
ValidatePasswordState ValidatePassword(const QString& password);

}  // namespace sysinfo

#endif  // DEEPIN_INSTALLER_REBORN_SYSINFO_VALIDATE_PASSWORD_H