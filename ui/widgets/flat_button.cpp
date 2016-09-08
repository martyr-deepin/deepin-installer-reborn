// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/flat_button.h"

#include "base/file_util.h"
#include "ui/utils/xutil.h"

namespace ui {

FlatButton::FlatButton(QWidget* parent) : QPushButton(parent) {
  this->setFlat(true);
  this->setStyleSheet(
      base::ReadTextFileContent(QStringLiteral(":/styles/flat_button.css")));
}

FlatButton::FlatButton(const QString& text, QWidget* parent)
    : FlatButton(parent) {
  this->setText(text);
}

void FlatButton::enterEvent(QEvent* event) {
  utils::SetCursorShape(this, XC_hand1);
  QPushButton::enterEvent(event);
}

void FlatButton::leaveEvent(QEvent* event) {
  utils::ResetCursorShape(this);
  QPushButton::leaveEvent(event);
}

}  // namespace ui