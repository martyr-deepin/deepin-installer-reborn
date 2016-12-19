// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/thread_util.h"

#include <QThread>

namespace installer {

void QuitThread(QThread* thread) {
  Q_ASSERT(thread);
  if (thread) {
    thread->quit();
    if (!thread->wait(3)) {
      thread->terminate();
      thread->wait();
    }
  }
}

}  // namespace installer