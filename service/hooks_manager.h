// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_HOOKS_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_HOOKS_MANAGER_H

#include <QObject>

namespace service {

// HookManager is used to do:
//   * run hook jobs one by one;
//   * load oem hooks;
//   * manage chroot environment;
//   * manage installation process;
//   * manage installation error message;
class HooksManager : public QObject {
  Q_OBJECT

 public:
  explicit HooksManager(QObject* parent = nullptr);

 signals:
  // Installation process is split into four stages:
  //   * disk partition: 0-15
  //   * before_chroot: 15-60
  //   * in_chroot: 60-80
  //   * after_chroot: 80-100
  void processUpdate(int process);
};

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_HOOKS_MANAGER_H
