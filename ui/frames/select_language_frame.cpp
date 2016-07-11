// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/select_language_frame.h"

namespace ui {

SelectLanguageFrame::SelectLanguageFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("select_language_frame"));
}

}  // namespace ui