// Copyright (c) 2017 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_SIMPLE_DISK_BUTTON_H
#define INSTALLER_UI_WIDGETS_SIMPLE_DISK_BUTTON_H

#include "ui/widgets/pointer_button.h"
class QLabel;

#include "partman/device.h"

namespace installer {

class SimpleDiskButton : public PointerButton {
  Q_OBJECT
 public:
  SimpleDiskButton(const Device& device, QWidget* parent = nullptr);

  const Device& device() const { return device_; }

  // Get device button selection state.
  bool selected() const { return selected_; }

  // Set whether current partition is selected.
  void setSelected(bool selected);

 private:
  void initUI();

  const Device& device_;
  QLabel* os_label_ = nullptr;
  bool selected_ = false;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_SIMPLE_DISK_BUTTON_H
