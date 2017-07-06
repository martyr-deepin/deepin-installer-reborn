// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_hostname.h"

#include <QRegExp>

namespace installer {

ValidateHostnameState ValidateHostname(const QString& hostname,
                                       const QStringList& reserved) {
  if (hostname.isEmpty()) {
    return ValidateHostnameState::EmptyError;
  }
  if (hostname.length() < kHostnameMinLen) {
    return ValidateHostnameState::TooShortError;
  }
  if (hostname.length() > kHostnameMaxLen) {
    return ValidateHostnameState::TooLongError;
  }
  if (reserved.contains(hostname)) {
    return ValidateHostnameState::ReservedError;
  }

  const QRegExp reg("[a-z0-9-]{1,63}", Qt::CaseInsensitive);
  const QStringList parts = hostname.split('.');
  if (parts.isEmpty()) {
    return ValidateHostnameState::InvalidChar;
  }

  for (const QString& part : parts) {
    if (part.startsWith('-') ||
        part.endsWith('-') ||
        !reg.exactMatch(part)) {
      return ValidateHostnameState::InvalidChar;
    }
  }

  return ValidateHostnameState::Ok;
}

}  // namespace installer