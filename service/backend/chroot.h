// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_BACKEND_CHROOT_H
#define INSTALLER_SERVICE_BACKEND_CHROOT_H

namespace installer {

// A helper class to enter/leave chroot environment.
// Separated from HooksManager class.
class Chroot {
 public:
  Chroot();

  // Chroot to |chroot_dir|.
  bool enterChroot(const char* chroot_dir);
  bool leaveChroot();

  // Whether current process is in chroot environment.
  bool inChrootEnv() const;

 private:
  // A file descriptor object used to escape chroot environment.
  int chroot_fd_;
};

}  // namespace installer

#endif  // INSTALLER_SERVICE_BACKEND_CHROOT_H
