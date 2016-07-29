// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/prepare_install_frame.h"

namespace ui {

PrepareInstallFrame::PrepareInstallFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("prepare_install_frame"));

  this->initUI();
  this->initConnections();
}

void PrepareInstallFrame::initConnections() {

}

void PrepareInstallFrame::initUI() {

}

}  // namespace ui