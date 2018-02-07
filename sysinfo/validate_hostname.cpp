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