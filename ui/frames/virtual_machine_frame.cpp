// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/virtual_machine_frame.h"

namespace ui {

VirtualMachineFrame::VirtualMachineFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("virtual_machine_frame"));
}

}  // namespace ui