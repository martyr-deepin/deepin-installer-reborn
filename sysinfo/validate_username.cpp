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

#include "sysinfo/validate_username.h"

#include <QDebug>
#include <QRegExp>

#include "base/file_util.h"

namespace installer {

namespace {

// Check whether |username| is in reserved username list.
bool IsReservedUsername(const QString& username,
                        const QString& reserved_username_file) {
  const QString content = ReadFile(reserved_username_file);
  if (content.isEmpty()) {
    qWarning() << "Reserved username list is empty";
    return false;
  }

  const QStringList lines = content.split('\n');
  for (const QString& line : lines) {
    if (line.isEmpty() || line.startsWith('#')) {
      continue;
    }
    if (line == username) {
      return true;
    }
  }

  return false;
}

}  // namespace

ValidateUsernameState ValidateUsername(const QString& username,
                                       const QString& reserved_username_file,
                                       int min_len,
                                       int max_len) {
  if (username.isEmpty() && (min_len > 0)) {
    return ValidateUsernameState::EmptyError;
  }
  if (username.length() < min_len) {
    return ValidateUsernameState::TooShortError;
  }
  if (username.length() > max_len) {
    return ValidateUsernameState::TooLongError;
  }

  const uint first_char = username.at(0).unicode();
  if (first_char < 'a' || first_char > 'z') {
    return ValidateUsernameState::FirstCharError;
  }

  const QRegExp reg("[a-z][a-z0-9_-]*");
  if (!reg.exactMatch(username)) {
    return ValidateUsernameState::InvalidCharError;
  }

  // TODO(xushaohua): Add reserved username list to settings.ini
  if (IsReservedUsername(username, reserved_username_file)) {
    return ValidateUsernameState::ReservedError;
  }

  return ValidateUsernameState::Ok;
}

}  // namespace installer
