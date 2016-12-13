// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H
#define INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H

#include <QString>

namespace installer {

// Encode |msg| with base64()
QString EncodeErrorMsg(const QString& msg);

// Get animation options used in install_progress_frame.
void GetInstallProgressFrameAnimationLevel(bool& position_animation,
                                           bool& opacity_animation);

// Get number of visible pages displayed in main page.
int GetVisiblePages();

// Check whether disk device is large enough enough to install new system.
bool IsDiskSpaceInsufficient(int required_device_size);

// Check whether partition table matches machine settings.
bool IsPartitionTableMatch();

// Read log file content, stripped to tail, and encode with domain name of
// feedback server.
// Returns false if failed.
bool ReadErrorMsg(QString& msg, QString& encoded_msg);

// Copy log file from memory to disk.
bool SaveLogFileToDisk();

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_MAIN_WINDOW_UTIL_H
