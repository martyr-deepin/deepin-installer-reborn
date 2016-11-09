// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/command.h"

#include <QDebug>
#include <QDir>
#include <QProcess>

namespace installer {

bool RunScriptFile(const QString& filepath, bool change_working_dir) {
  QString output, err;
  return RunScriptFile(filepath, output, err, change_working_dir);
}

bool RunScriptFile(const QString& filepath, QString& output, QString& err,
                   bool change_working_dir) {
  return RunScriptFile(filepath, {}, output, err, change_working_dir);
}

bool RunScriptFile(const QString& filepath, const QStringList& args,
                   QString& output, QString& err, bool change_working_dir) {
  if (change_working_dir) {
    // Change working directory.
    const QString current_dir(QFileInfo(filepath).absolutePath());
    if (!QDir::setCurrent(current_dir)) {
      qCritical() << "Failed to change working directory:" << current_dir;
      return false;
    }
  }

  QProcess process;
  process.setProgram(filepath);
  process.setArguments(args);
  process.start();
  // Wait for process to finish without timeout.
  process.waitForFinished(-1);
  output = process.readAllStandardOutput();
  err = process.readAllStandardError();
  return (process.exitStatus() == QProcess::NormalExit &&
          process.exitCode() == 0);
}

bool SpawnCmd(const QString& cmd, const QStringList& args) {
  QString output;
  QString err;
  return SpawnCmd(cmd, args, output, err);
}

bool SpawnCmd(const QString& cmd, const QStringList& args, QString& output) {
  QString err;
  return SpawnCmd(cmd, args, output, err);
}

bool SpawnCmd(const QString& cmd, const QStringList& args,
              QString& output, QString& err) {
  QProcess process;
  process.setProgram(cmd);
  process.setArguments(args);
  process.start();
  process.waitForFinished();
  output = process.readAllStandardOutput();
  err = process.readAllStandardError();
  return (process.exitStatus() == QProcess::NormalExit &&
          process.exitCode() == 0);
}

}  // namespace installer