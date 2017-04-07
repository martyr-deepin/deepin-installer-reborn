// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
