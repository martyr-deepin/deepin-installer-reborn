// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/system_info_frame.h"

namespace ui {

SystemInfoFrame::SystemInfoFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("system_info_frame"));
}

void SystemInfoFrame::onNextButtonClicked() {

}

}  // namespace ui