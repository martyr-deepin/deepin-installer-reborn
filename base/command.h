// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_BASE_COMMAND_H
#define DEEPIN_INSTALLER_REBORN_BASE_COMMAND_H

#include <QString>

namespace base {

// Run a script file at |filepath|.
// If |change_working_dir| is true, current working directory is changed to
// folder of |filepath|.
// Returns true if |filepath| executed and exited with 0.
bool RunScriptFile(const QString& filepath, bool change_working_dir = true);

}  // namespace base

#endif  // DEEPIN_INSTALLER_REBORN_BASE_COMMAND_H
