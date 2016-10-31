// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>
#include <QFrame>

#include "ui/delegates/partition_delegate.h"
#include "ui/frames/inner/simple_partition_frame.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QFrame window;
  installer::PartitionDelegate delegate;
  installer::SimplePartitionFrame frame(&delegate, &window);
  delegate.scanDevices();
  window.resize(640, 480);
  window.show();
  window.setStyleSheet("background: transparent;");

  return app.exec();
}