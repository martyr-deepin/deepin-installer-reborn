// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_failed_frame.h"

namespace ui {

InstallFailedFrame::InstallFailedFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("install_failed_frame"));
}

}  // namespace ui