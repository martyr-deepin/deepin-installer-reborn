// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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