// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_hostname.h"

#include <QRegExp>

namespace installer {

ValidateHostnameState ValidateHostname(const QString& hostname,
                                       int min_len,
                                       int max_len,
                                       const QStringList& reserved) {
  Q_ASSERT(min_len >= 0);
  Q_ASSERT(max_len >= min_len);

  if (hostname.isEmpty() && min_len > 0) {
    return ValidateHostnameState::EmptyError;
  }
  if (hostname.length() < min_len) {
    return ValidateHostnameState::TooShortError;
  }
  if (hostname.length() > max_len) {
    return ValidateHostnameState::TooLongError;
  }
  if (reserved.contains(hostname)) {
    return ValidateHostnameState::ReservedError;
  }

  const QRegExp reg(QStringLiteral("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9-]*"
      "[a-zA-Z0-9]).)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9-]*[A-Za-z0-9])$"));
  if (!reg.exactMatch(hostname)) {
    return ValidateHostnameState::InvalidChar;
  }

  return ValidateHostnameState::Ok;
}

bool ValidateHostnameTemp(const QString& hostname) {
  const QRegExp reg(QStringLiteral("^([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9-]*"
                                   "[a-zA-Z0-9-.])*$"));
  return reg.exactMatch(hostname);
}

}  // namespace installer