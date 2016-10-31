// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_POINTER_BUTTON_H
#define INSTALLER_POINTER_BUTTON_H

#include <QPushButton>

namespace installer {

// Change cursor shape when mouse moves over button
class PointerButton : public QPushButton {
  Q_OBJECT

 public:
  explicit PointerButton(QWidget* parent = nullptr);
  PointerButton(const QString& text, QWidget* parent = nullptr);

 protected:
  // Override these two event handlers to implements hover effect.
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;
};

}  // namespace installer

#endif  // INSTALLER_POINTER_BUTTON_H
