// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_HOOKS_MANAGER_H
#define INSTALLER_SERVICE_HOOKS_MANAGER_H

#include <QObject>
#include <QStringList>
class QTimer;

namespace installer {

// Expose this value explicitly.
const int kBeforeChrootStartVal = 5;

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
  void initConnections();

  enum class HookType {
    BeforeChroot,
    InChroot,
    AfterChroot,
  };

  // Mount hooks folder to /dev/shm
  bool bindHooks(HookType hook_type);
  // Umount hooks folder.
  bool unbindHooks();
  // Returns a list of sorted hook scripts with |hook_type|.
  QStringList listHooks(HookType hook_type);
  // Run hook scripts with |hook_type|.
  bool runHooksPack(HookType hook_type, int progress_begin, int progress_end);

  bool enterChroot();
  bool leaveChroot();
  // A file descriptor object used to escape chroot environment.
  int chroot_fd_;

  // Monitors unsquashfs progress file changing.
  void monitorProgressFiles();
  // This timer is used to read progress file each second.
  QTimer* unsquashfs_timer_ = nullptr;
  enum class UnsquashfsStage {
    UnInitialized,
    ReadBase,

    // TODO(xushaohua): Do not read process of language.module.
    ReadLang,
  };
  UnsquashfsStage unsquashfs_stage_;
  bool overlay_filesystem_exists_;

 private slots:
  void handleRunHooks();
  void handleReadUnsquashfsTimeout();
};

}  // namespace installer

#endif  // INSTALLER_SERVICE_HOOKS_MANAGER_H
