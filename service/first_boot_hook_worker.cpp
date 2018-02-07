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