// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QApplication>

#include "ui/tests/animations/animations_main_window.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  installer::AnimationsMainWindow window;
  window.show();

  return app.exec();
}