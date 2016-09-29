// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_FROSTED_FRAME_H
#define INSTALLER_UI_WIDGETS_FROSTED_FRAME_H

#include <QFrame>

namespace installer {

// Displays a semi-transparent frame with round corners.
class FrostedFrame : public QFrame {
  Q_OBJECT

 public:
 explicit FrostedFrame(QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_FROSTED_FRAME_H
