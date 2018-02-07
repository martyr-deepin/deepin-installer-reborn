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

#ifndef INSTALLER_UI_XRANDR_MULTI_HEAD_WORKER_H
#define INSTALLER_UI_XRANDR_MULTI_HEAD_WORKER_H

#include <QObject>

namespace installer {

// Monitors XRR change notify
class MultiHeadWorker : public QObject {
  Q_OBJECT

 public:
  explicit MultiHeadWorker(QObject* parent = nullptr);
  ~MultiHeadWorker();

 public slots:
  // Stop monitoring process.
  // This method can be called in another thread.
  // FIXME(xushaohua): Stop the event loop explicitly.
  void stop();

 signals:
  // Emitted when number or order of screens changed.
  void monitorsChanged();

  void start();

 private:
  void initConnections();
  bool is_running_;

 private slots:
  void doStart();
};

}  // namespace installer

#endif  // INSTALLER_UI_XRANDR_MULTI_HEAD_WORKER_H
