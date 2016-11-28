// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UNSQUASHFS_GUI_WORKER_H_
#define INSTALLER_UNSQUASHFS_GUI_WORKER_H_

#include <QRunnable>
#include <QString>

class Worker : public QRunnable {
 public:
  explicit Worker(const QString& squashfs_file);

  void run() override;

 private:
  QString squashfs_file_;
};

#endif  // INSTALLER_UNSQUASHFS_GUI_WORKER_H_
