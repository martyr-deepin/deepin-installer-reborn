// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/command.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

namespace base {

bool RunScriptFile(const QString& filepath, bool change_working_dir) {
// Change working directory.
  if (change_working_dir) {
    if (!QDir::setCurrent(QFileInfo(filepath).absolutePath())) {
      qCritical() << "Failed to change working directory:"
                  << filepath;
      return false;
    }
  }
  const QStringList args = { filepath };
  return (QProcess::execute(filepath, args) == 0);
}

}  // namespace base