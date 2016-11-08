// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_BACKEND_HOOK_WORKER_H
#define INSTALLER_SERVICE_BACKEND_HOOK_WORKER_H

#include <QObject>

namespace installer {

// Run hook script in background thread.
class HookWorker : public QObject {
  Q_OBJECT

 public:
  explicit HookWorker(QObject* parent = nullptr);

 signals:
  // Notify this worker to run another |hook|.
  // Emit this signal only after receiving hooksFinished() signal.
  void runHook(const QString& hook);

  // Emitted when current hook finished with result |ok|.
  void hookFinished(bool ok);

 private slots:
  void handleRunHook(const QString& hook);
};

}  // namespace installer

#endif  // INSTALLER_SERVICE_BACKEND_HOOK_WORKER_H
