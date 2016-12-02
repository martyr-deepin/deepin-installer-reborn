// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/backend/hook_worker.h"

#include <QDebug>

#include "base/command.h"

namespace installer {

namespace {

const char kInstallerHooksDir[] = "/dev/shm/installer-hooks";
const char kHookManagerFile[] = "/dev/shm/installer-hooks/hook-manager.sh";

// Runs a specific hook at |hook|.
bool RunHook(const QString& hook) {
  qDebug() << "RunHook():" << hook;
  QString output, err;
  const QStringList args = {hook};
  const bool ok = RunScriptFile(kHookManagerFile, args, output, err, true);
  if (!output.isEmpty()) {
    qWarning() << "Output from " << hook << "\n" << output;
  }
  if (!err.isEmpty()) {
    qCritical() << "Error from " << hook << "\n" <<err;
  }
  return ok;
}

}  // namespace

HookWorker::HookWorker(QObject* parent) : QObject(parent) {
  this->setObjectName("hook_worker");
  connect(this, &HookWorker::runHook,
          this, &HookWorker::handleRunHook);
}

void HookWorker::handleRunHook(const QString& hook) {
  const bool ok = RunHook(hook);
  emit this->hookFinished(ok);
}

}  // namespace installer