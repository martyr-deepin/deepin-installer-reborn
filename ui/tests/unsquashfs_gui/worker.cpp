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

#include "ui/tests/unsquashfs_gui/worker.h"

#include <QDebug>
#include <QDir>

#include "base/command.h"

Worker::Worker(const QString& squashfs_file)
    : QRunnable(),
      squashfs_file_(squashfs_file){
}

void Worker::run() {
  // Change working directory to folder in which squashfs_file_ is located.
  QDir::setCurrent(QFileInfo(squashfs_file_).absolutePath());
  installer::RunScriptFile({UNSQUASHFS_SH, squashfs_file_});
}
