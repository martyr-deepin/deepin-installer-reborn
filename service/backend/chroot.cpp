// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/chroot.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <QDebug>

namespace installer {

namespace {

const int kInvalidFd = -1;

}  // namespace

Chroot::Chroot() : chroot_fd_(kInvalidFd) {

}

bool Chroot::enterChroot(const char* chroot_dir) {
  if (chroot_fd_ != kInvalidFd) {
    qWarning() << "enterChroot() already in chroot env!";
    return false;
  }

  chroot_fd_ = open("/", O_RDONLY);
  if (chroot_fd_ == kInvalidFd) {
    qCritical() << "enterChroot() failed to call open(/)" << strerror(errno);
    return false;
  }

  if (chdir(chroot_dir) != 0) {
    qCritical() << "chdir() failed:" << chroot_dir << strerror(errno);
    chroot_fd_ = kInvalidFd;
    return false;
  }

  if (chroot(chroot_dir) != 0) {
    qCritical() << "Call chroot() failed:" << chroot_dir << strerror(errno);
    chroot_fd_ = kInvalidFd;
    return false;
  }

  if (chdir("/") != 0) {
    qCritical() << "Failed to call chdir(/)";
    chroot_fd_ = kInvalidFd;
    return false;
  }
  return true;
}

bool Chroot::leaveChroot() {
  if (chroot_fd_ == kInvalidFd) {
    qWarning() << "leaveChroot() not in chroot env!";
    return false;
  }

  if (fchdir(chroot_fd_) != 0) {
    qCritical() << "fchdir() failed, fd:" << chroot_fd_;
    return false;
  }

  // Chroot to host environment.
  if (chroot(".") != 0) {
    qCritical() << "Failed to call chroot(.)";
    return false;
  }

  chroot_fd_ = kInvalidFd;
  return true;
}

}  // namespace