// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_HOOKS_MANAGER_H
#define INSTALLER_SERVICE_HOOKS_MANAGER_H

#include <QObject>
class QThread;
class QTimer;

#include "service/backend/chroot.h"

namespace installer {

// Expose this value explicitly.
const int kBeforeChrootStartVal = 5;

class HooksPack;
class HookWorker;

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
  ~HooksManager();

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
  void initConnections();

  void runHook();

  // Run hook scripts with |hook_type|.
  void runHooksPack();

  HooksPack* hooks_pack_ = nullptr;
  HookWorker* hook_worker_ = nullptr;
  QThread* hook_worker_thread_ = nullptr;
  Chroot chroot_;

  // Monitors unsquashfs progress file changing.
  void monitorProgressFiles();

  // This timer is used to read progress file each second.
  QTimer* unsquashfs_timer_ = nullptr;

 private slots:
  void handleRunHooks();
  void handleReadUnsquashfsTimeout();

  // Handles any errors.
  void onHooksManagerFinished();

  // Run next hook when current hook has finished.
  void onHookFinished(bool ok);
};

}  // namespace installer

#endif  // INSTALLER_SERVICE_HOOKS_MANAGER_H
