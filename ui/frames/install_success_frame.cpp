// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_success_frame.h"

namespace ui {

InstallSuccessFrame::InstallSuccessFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("install_success_frame"));
}

}  // namespace ui