// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_VALIDATE_HOSTNAME_H
#define INSTALLER_SYSINFO_VALIDATE_HOSTNAME_H

#include <QString>
#include <QStringList>

namespace installer {

enum class ValidateHostnameState {
  Ok,
  EmptyError,
  InvalidChar,
  ReservedError,
  TooLongError,
  TooShortError,
};

const int kHostnameMinLen = 1;
const int kHostnameMaxLen = 253;

// Check hostname (machine name) based on the following rules:
//   * Only letters(a-z, A-Z) and numbers(0-9) can be used as prefix/suffix;
//   * Dot(.) is used to separate domain and sub-domain;
//   * Underscore(_) and dash(-) are used to concat letters and numbers.
// Parameter list:
//  * |hostname|, hostname to be validated.
//  * |reserved|, a list of hostname reserved by system, normally it only
//    contains "localhost".
// For more information about hostname, please visit:
//  * https://en.wikipedia.org/wiki/Hostname
//  * https://tools.ietf.org/html/rfc1123#page-13
ValidateHostnameState ValidateHostname(const QString& hostname,
                                       const QStringList& reserved);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_VALIDATE_HOSTNAME_H
