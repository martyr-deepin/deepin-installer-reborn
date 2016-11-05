// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_VALIDATE_USERNAME_H
#define INSTALLER_SYSINFO_VALIDATE_USERNAME_H

#include <QString>

namespace installer {

enum class ValidateUsernameState {
  Ok,
  EmptyError,  // Username is empty.
  FirstCharError,  // First character of username is not in a-z.
  InvalidCharError,  // Contains invalid word.
  ReservedError,  // Username already exists.
  TooLongError,
  TooShortError,
};

// Check whether |username| is appropriate.
// The following rules are checked by order:
//   * Length of |username| is [min_len, max_len];
//   * |username| can only contain lower letters(a-z), numbers(0-9), dash(-)
//     and underscore(_);
//   * First character of |username| must be a letter;
//   * |username| cannot be used by system (uid < 1000).
ValidateUsernameState ValidateUsername(const QString& username,
                                       int min_len,
                                       int max_len);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_VALIDATE_USERNAME_H
