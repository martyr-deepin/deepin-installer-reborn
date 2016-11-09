// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_BASE_COMMAND_H
#define INSTALLER_BASE_COMMAND_H

#include <QStringList>

namespace installer {

// Run a script file at |filepath|.
// If |change_working_dir| is true, current working directory is changed to
// folder of |filepath|.
// Returns true if |filepath| executed and exited with 0.
bool RunScriptFile(const QString& filepath, bool change_working_dir = true);
bool RunScriptFile(const QString& filepath, QString& output, QString& err,
                   bool change_working_dir = true);
bool RunScriptFile(const QString& filepath, const QStringList& args,
                   QString& output, QString& err,
                   bool change_working_dir = true);

// Run |cmd| with |args| in background and returns its result.
bool SpawnCmd(const QString& cmd, const QStringList& args);
bool SpawnCmd(const QString& cmd, const QStringList& args, QString& output);
bool SpawnCmd(const QString& cmd, const QStringList& args, QString& output,
              QString& err);

}  // namespace installer

#endif  // INSTALLER_BASE_COMMAND_H
