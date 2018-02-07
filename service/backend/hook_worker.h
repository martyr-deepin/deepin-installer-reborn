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
