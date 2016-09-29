// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/validate_hostname.h"

#include <QRegExp>

namespace installer {

bool ValidateHostname(const QString& hostname) {
  if (hostname.isEmpty()) {
    return false;
  }
  const QRegExp reg(QStringLiteral("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9-]*"
      "[a-zA-Z0-9]).)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9-]*[A-Za-z0-9])$"));
  return reg.exactMatch(hostname);
}

bool ValidateHostnameTemp(const QString& hostname) {
  const QRegExp reg(QStringLiteral("^([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9-]*"
                                   "[a-zA-Z0-9-.])*$"));
  return reg.exactMatch(hostname);
}

}  // namespace installer