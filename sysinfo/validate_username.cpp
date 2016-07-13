// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_username.h"

#include <QRegExp>

#include "sysinfo/users.h"

namespace sysinfo {

ValidateUsernameState ValidateUsername(const QString& username) {
  if (username.isEmpty()) {
    return ValidateUsernameState::EmptyError;
  }

  if (username.length() > kUsernameMaximumLen) {
    return ValidateUsernameState::TooLongError;
  }

  const uint first_char = username.at(0).unicode();

  if (first_char < 'a' || first_char > 'z') {
    return ValidateUsernameState::FirstCharError;
  }

  const QRegExp reg(QStringLiteral("[a-z][a-z0-9_-]*"));
  if (!reg.exactMatch(username)) {
    return ValidateUsernameState::InvalidCharError;
  }

  if (GetUsers().contains(username)) {
    return ValidateUsernameState::AlreadyUsedError;
  }

  return ValidateUsernameState::Ok;
}

}  // namespace sysinfo