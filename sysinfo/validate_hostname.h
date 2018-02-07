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
