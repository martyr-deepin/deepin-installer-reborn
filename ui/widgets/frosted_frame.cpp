// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/frosted_frame.h"

#include "base/file_util.h"

namespace installer {

FrostedFrame::FrostedFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("frosted_frame"));
  this->setStyleSheet(ReadTextFileContent(":/styles/frosted_frame.css"));
}

}  // namespace installer