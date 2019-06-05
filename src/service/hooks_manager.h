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

#ifndef INSTALLER_SERVICE_HOOKS_MANAGER_H
#define INSTALLER_SERVICE_HOOKS_MANAGER_H

#include <QObject>
#include <utility>

class QThread;
class QTimer;

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

  void runNextHook();

  // Run hook scripts with |hook_type|.
  void runHooksPack();

  HooksPack* hooks_pack_ = nullptr;
  HookWorker* hook_worker_ = nullptr;
  QThread* hook_worker_thread_ = nullptr;

  // Monitors unsquashfs progress file changing.
  void monitorProgressFiles();

  // This timer is used to read progress file each second.
  QTimer* unsquashfs_timer_ = nullptr;

  // Recored the script run time
  bool enableScriptAnalyze;
  qlonglong lastRunTime;
  QList<std::pair<QString, qlonglong>> scriptRunTimeList;

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
