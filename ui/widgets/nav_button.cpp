// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/nav_button.h"

#include "base/file_util.h"

namespace ui {

NavButton::NavButton(const QString& text, QWidget* parent)
  : FlatButton(text, parent) {
  this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  this->setStyleSheet(
      base::ReadTextFileContent(QStringLiteral(":/styles/nav_button.css")));
}

}  // namespace ui