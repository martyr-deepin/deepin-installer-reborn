// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_LOG_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_LOG_MANAGER_H

#include <QString>

namespace service {

// If current user has no write permission in /var/log, keep log file in /tmp.
// TODO(xushaohua): Rotate log file.
QString GetLogFilepath();

// Initialize console log and file log.
void InitLogService();

}  // namespace service


#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_LOG_MANAGER_H
