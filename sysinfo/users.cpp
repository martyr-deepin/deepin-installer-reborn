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

#include "sysinfo/users.h"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

namespace installer {

namespace {

// Minimum uid used for normal user.
const int kNormalUserMinUid = 1000;

}  // namespace

QStringList GetSystemUsers() {
  QStringList users;
  struct passwd* pwd;
  while ((pwd = getpwent()) != NULL) {
    if (pwd->pw_uid < kNormalUserMinimumUid) {
      users.append(pwd->pw_name);
    }
  }
  endpwent();

  return users;
}

QStringList GetUsers() {
  QStringList users;
  struct passwd* pwd;
  while ((pwd = getpwent()) != NULL) {
    users.append(pwd->pw_name);
  }
  endpwent();

  return users;
}

bool HasRootPrivilege() {
  return geteuid() == 0;
}

}  // namespace installer