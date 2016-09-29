// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_VALIDATE_USERNAME_H
#define INSTALLER_SYSINFO_VALIDATE_USERNAME_H

#include <QString>

namespace installer {

enum class ValidateUsernameState {
  Ok,
  AlreadyUsedError,  // Username already exists.
  EmptyError,  // Username is empty.
  FirstCharError,  // First character of username is not in a-z.
  InvalidCharError,  // Contains invalid word.
  TooLongError,  // Contains more than 32 chars.
};

const int kUsernameMaximumLen = 32;

// Check whether |username| is appropriate.
// The following rules are checked by order:
//   * Length of |username| is (0, 32];
//   * |username| can only contain lower letters(a-z), numbers(0-9), dash(-)
//     and underscore(_);
//   * First character of |username| must be a letter;
//   * |username| cannot be used by others or by system.
ValidateUsernameState ValidateUsername(const QString& username);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_VALIDATE_USERNAME_H
