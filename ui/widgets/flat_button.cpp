// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/flat_button.h"

#include "base/file_util.h"
#include "ui/utils/xutil.h"

namespace installer {

FlatButton::FlatButton(QWidget* parent) : PointerButton(parent) {
  this->setFlat(true);
  this->setStyleSheet(ReadTextFileContent(":/styles/flat_button.css"));
}

FlatButton::FlatButton(const QString& text, QWidget* parent)
    : FlatButton(parent) {
  this->setText(text);
}

}  // namespace installer