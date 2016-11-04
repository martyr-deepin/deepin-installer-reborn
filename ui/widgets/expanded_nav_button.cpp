// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/expanded_nav_button.h"

#include "base/file_util.h"

namespace installer {

ExpandedNavButton::ExpandedNavButton(const QString& text, QWidget* parent)
  : NavButton(text, parent) {
  this->setObjectName("expanded_nav_button");

  // Remove width constraints in NavButton.
  this->setFixedWidth(QWIDGETSIZE_MAX);
  this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

}  // namespace installer