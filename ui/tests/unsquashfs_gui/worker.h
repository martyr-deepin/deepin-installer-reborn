// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_TESTS_UNSQUASHFS_GUI_WORKER_H
#define INSTALLER_UI_TESTS_UNSQUASHFS_GUI_WORKER_H

#include <QRunnable>
#include <QString>

class Worker : public QRunnable {
 public:
  explicit Worker(const QString& squashfs_file);

  void run() override;

 private:
  QString squashfs_file_;
};

#endif  // INSTALLER_UI_TESTS_UNSQUASHFS_GUI_WORKER_H
