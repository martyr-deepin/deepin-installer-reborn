// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_LOG_MANAGER_H
#define INSTALLER_SERVICE_LOG_MANAGER_H

#include <QString>

namespace installer {

// If current user has no write permission in /var/log, keep log file in /tmp.
QString GetLogFilepath();

// Redirect stdout and stderr to |log_file|.
bool RedirectLog(const QString& log_file);

}  // namespace installer


#endif  // INSTALLER_SERVICE_LOG_MANAGER_H
