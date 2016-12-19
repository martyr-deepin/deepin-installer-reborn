// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_BASE_THREAD_UTIL_H
#define INSTALLER_BASE_THREAD_UTIL_H

class QThread;

namespace installer {

// Quit QThread object and wait for 3s.
// If it is still running, terminate it.
void QuitThread(QThread* thread);

}  // namespace installer

#endif  // INSTALLER_BASE_THREAD_UTIL_H
