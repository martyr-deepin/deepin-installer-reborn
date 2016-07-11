// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/disk_space_insufficient_frame.h"

namespace ui {

DiskSpaceInsufficientFrame::DiskSpaceInsufficientFrame(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName(QStringLiteral("disk_space_insufficient_frame"));
}

}  // namespace ui