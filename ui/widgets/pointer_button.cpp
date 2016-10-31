// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/pointer_button.h"

#include "ui/utils/xutil.h"

namespace installer {

PointerButton::PointerButton(QWidget* parent) : QPushButton(parent) {
  this->setObjectName(QStringLiteral("pointer_button"));
}

PointerButton::PointerButton(const QString& text, QWidget* parent)
    : PointerButton(parent) {
  this->setText(text);
}

void PointerButton::enterEvent(QEvent* event) {
  utils::SetCursorShape(this, XC_hand1);
  QPushButton::enterEvent(event);
}

void PointerButton::leaveEvent(QEvent* event) {
  utils::ResetCursorShape(this);
  QPushButton::leaveEvent(event);
}

}  // namespace installer