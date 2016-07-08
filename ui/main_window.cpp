// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/main_window.h"

namespace ui {

MainWindow::MainWindow() : QWidget() {
  this->setObjectName(QStringLiteral("main_window"));
}

}  // namespace ui