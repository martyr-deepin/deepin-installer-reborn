// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/select_bootloader_frame.h"

namespace ui {

SelectBootloaderFrame::SelectBootloaderFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("select_bootloader_frame"));

  this->initUI();
  this->initConnections();
}

void SelectBootloaderFrame::initConnections() {

}

void SelectBootloaderFrame::initUI() {

}

}  // namespace ui