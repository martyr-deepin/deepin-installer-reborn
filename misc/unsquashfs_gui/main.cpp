// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// This simple program is used to test reading progress value of `unsquashfs`
// command within GUI.

#include <QApplication>

#include "misc/unsquashfs_gui/unsquashfs_progress_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  
  UnsquashfsProgressWindow window;
  window.show();
  
  return app.exec();
}