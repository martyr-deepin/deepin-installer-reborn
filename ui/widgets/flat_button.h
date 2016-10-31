// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_FLAT_BUTTON_H
#define INSTALLER_UI_WIDGETS_FLAT_BUTTON_H

#include "ui/widgets/pointer_button.h"

namespace installer {

// FlatButton only displays an icon, without any frame lines.
// It also changes cursor shape.
class FlatButton : public PointerButton {
  Q_OBJECT

 public:
  explicit FlatButton(QWidget* parent = nullptr);
  FlatButton(const QString& text, QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_FLAT_BUTTON_H
