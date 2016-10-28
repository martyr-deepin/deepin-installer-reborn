// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
  void stop();

 signals:
  void screenCountChanged();

  void start();

 private:
  void initConnections();
  bool is_running_;

 private slots:
  void doStart();
};

}  // namespace installer

#endif  // INSTALLER_UI_XRANDR_MULTI_HEAD_WORKER_H
