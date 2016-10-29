// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_EXPANDED_NAV_BUTTON_H
#define INSTALLER_UI_WIDGETS_EXPANDED_NAV_BUTTON_H

#include "ui/widgets/nav_button.h"

namespace installer {

// Resizable NavButton.
class ExpandedNavButton : public NavButton {
  Q_OBJECT

 public:
  ExpandedNavButton(const QString& text, QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_EXPANDED_NAV_BUTTON_H
