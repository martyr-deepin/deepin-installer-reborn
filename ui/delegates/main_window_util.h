// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// Utilities used in MainWindow.

#ifndef INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H
#define INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H

#include <QString>

namespace installer {

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

// Check whether partition table matches machine settings.
bool IsPartitionTableMatch();

// Read log file content, stripped to tail, and encode with domain name of
// feedback server.
// Returns false if failed.
bool ReadErrorMsg(QString& msg, QString& encoded_msg);

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H
