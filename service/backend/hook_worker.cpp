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

#include "service/backend/hook_worker.h"

#include <QDebug>

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

namespace {

// Absolute path to hook_manager.sh
const char kHookManagerFile[] = BUILTIN_HOOKS_DIR "/hook_manager.sh";

// Runs a specific hook at |hook|.
bool RunHook(const QString& hook) {
  const QStringList args = {kHookManagerFile, hook};
  return RunScriptFile(args);
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