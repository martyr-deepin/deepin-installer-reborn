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
//   * |username| cannot be in |reserved_username_file|.
ValidateUsernameState ValidateUsername(const QString& username,
                                       const QString& reserved_username_file,
                                       int min_len,
                                       int max_len);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_VALIDATE_USERNAME_H
