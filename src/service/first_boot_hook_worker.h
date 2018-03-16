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
