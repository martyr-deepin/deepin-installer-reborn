// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_NAV_BUTTON_H
#define INSTALLER_UI_WIDGETS_NAV_BUTTON_H

#include "ui/widgets/flat_button.h"

namespace installer {

// NavButton is used to switch pages.
// Almost always displays at bottom of frame page.
class NavButton : public FlatButton {
  Q_OBJECT

 public:
  NavButton(const QString& text, QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_NAV_BUTTON_H
