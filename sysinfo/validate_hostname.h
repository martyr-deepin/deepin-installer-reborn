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

// Check hostname (machine name) based on the following rules:
//   * Only letters(a-z, A-Z) and numbers(0-9) can be used as prefix/suffix;
//   * Dot(.) is used to separate domain and sub-domain;
//   * Underscore(_) and dash(-) are used to concat letters and numbers.
// Parameter list:
//  * |hostname|, hostname to be validated.
//  * |min_len|, minimum characters allowed in |hostname|.
//  * |max_len|, maximum characters allowed in |hostname|.
//  * |reserved|, a list of hostname reserved by system, normally it only
//    contains "localhost".
ValidateHostnameState ValidateHostname(const QString& hostname,
                                       int min_len,
                                       int max_len,
                                       const QStringList& reserved);

// Validate part of hostname.
// Use this function when |hostname| is being edited.
// DEPRECATED
bool ValidateHostnameTemp(const QString& hostname);

}  // namespace installer

#endif  // INSTALLER_SYSINFO_VALIDATE_HOSTNAME_H
