// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_FIRST_BOOT_HOOK_WORKER_H
#define INSTALLER_SERVICE_FIRST_BOOT_HOOK_WORKER_H

#include <QObject>

namespace installer {

// A worker to execute first_boot_setup.sh and returns result.
class FirstBootHookWorker : public QObject {
  Q_OBJECT

 public:
  explicit FirstBootHookWorker(QObject* parent = nullptr);

 signals:
  // Emitted when background hook worker exited with state |ok|.
  void hookFinished(bool ok);

  // Notify hook manager to run hook worker in background.
  void startHook();

 private slots:
  void doStartHook();
};

}  // namespace installer

#endif  // INSTALLER_SERVICE_FIRST_BOOT_HOOK_WORKER_H
