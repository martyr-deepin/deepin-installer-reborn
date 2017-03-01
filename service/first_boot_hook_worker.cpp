// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/first_boot_hook_worker.h"

#include <QDebug>

#include "base/command.h"

namespace installer {

namespace {

// Absolute path to built-in first boot script.
const char kFirstBootHookFile[] = BUILTIN_HOOKS_DIR "/first_boot_setup.sh";

}  // namespace

FirstBootHookWorker::FirstBootHookWorker(QObject* parent) : QObject(parent) {
  this->setObjectName("first_boot_hook_worker");

  connect(this, &FirstBootHookWorker::startHook,
          this, &FirstBootHookWorker::doStartHook);
}

void FirstBootHookWorker::doStartHook() {
  QString out, err;
  const bool ok = RunScriptFile({kFirstBootHookFile}, out, err);
  if (!out.isEmpty()) {
    qWarning() << kFirstBootHookFile << "OUT:" << out;
  }
  if (!err.isEmpty()) {
    qCritical() << kFirstBootHookFile << "ERR:" << err;
  }

  emit this->hookFinished(ok);
}

}  // namespace installer