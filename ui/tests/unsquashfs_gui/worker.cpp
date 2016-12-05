// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "worker.h"

#include <QDebug>
#include <QDir>

#include "base/command.h"

Worker::Worker(const QString& squashfs_file) : QRunnable(),
                                               squashfs_file_(squashfs_file){
}

void Worker::run() {
  // Change working directory to folder in which squashfs_file_ is located.
  QDir::setCurrent(QFileInfo(squashfs_file_).absolutePath());
  QString output, err;
  installer::RunScriptFile(UNSQUASHFS_SH, QStringList(squashfs_file_),
                           output, err, false);
  qDebug() << "output:" << output;
  qDebug() << "err:" << err;
}
