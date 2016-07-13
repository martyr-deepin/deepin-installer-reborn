// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/header_label.h"

#include "base/file_util.h"

namespace ui {

HeaderLabel::HeaderLabel(const QString& text, QWidget* parent)
  : QLabel(text, parent) {
  this->setStyleSheet(
      base::ReadTextFileContent(QStringLiteral(":/styles/header_label.css")));
}

}  // namespace ui