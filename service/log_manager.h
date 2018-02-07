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
