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

// Utilities used in MainWindow.

#ifndef INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H
#define INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H

#include <QString>

namespace installer {

// Copy log file to |log_file|.
void CopyLogFile(const QString& log_file);

// Encode |msg| with base64()
QString EncodeErrorMsg(const QString& msg);

// Get number of visible pages displayed in main page.
// Typically there are four pages:
//  * SelectLanguageFrame
//  * SystemInfoFrame
//  * PartitionFrame
//  * InstallProgressFrame
int GetVisiblePages();

// Check whether disk space is large enough to install new system.
bool IsDiskSpaceInsufficient();

// Read log file content, stripped to tail, and encode with domain name of
// feedback server.
// Returns false if failed.
bool ReadErrorMsg(QString& msg, QString& encoded_msg);

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H
