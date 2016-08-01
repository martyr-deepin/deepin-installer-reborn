// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_HOOKS_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_HOOKS_MANAGER_H

#include <QObject>
#include <QStringList>

namespace service {

// HookManager is used to do:
//   * run hook jobs one by one;
//   * load oem hooks;
//   * manage chroot environment;
//   * manage installation process;
//   * manage installation error message;
// This object runs in background thread.
class HooksManager : public QObject {
  Q_OBJECT

 public:
  explicit HooksManager(QObject* parent = nullptr);

 signals:
  // Emitted when critical error has occurred.
  void errorOccurred();

  // Emitted when installation process finished successfully.
  void finished();

  // Installation process is split into four stages:
  //   * before_chroot: 5-60
  //   * in_chroot: 60-80
  //   * after_chroot: 80-100
  void processUpdate(int process);

  // Emit this signal in other objects to run hooks in background thread.
  void runHooks();

 private:
  enum class HookType {
    BeforeChroot,
    InChroot,
    AfterChroot,
  };

  // Mount hooks folder to /dev/shm
  bool bindHooks(HookType hook_type);
  bool unbindHooks();
  QStringList listHooks(HookType hook_type);
  bool runHooksPack(HookType hook_type, int progress_begin, int progress_end);

  bool enterChroot();
  bool leaveChroot();
  // A file descriptor object used to escape chroot environment.
  int chroot_fd_;

 private slots:
  void handleRunHooks();
};

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_HOOKS_MANAGER_H
