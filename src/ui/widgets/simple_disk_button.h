/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INSTALLER_UI_WIDGETS_SIMPLE_DISK_BUTTON_H
#define INSTALLER_UI_WIDGETS_SIMPLE_DISK_BUTTON_H

#include "ui/widgets/pointer_button.h"
class QLabel;

#include "partman/device.h"

namespace installer {

class SimpleDiskButton : public PointerButton {
  Q_OBJECT
 public:
  SimpleDiskButton(const Device::Ptr device, QWidget* parent = nullptr);

  const Device::Ptr device() const { return device_; }

  // Get device button selection state.
  bool selected() const { return selected_; }

  // Set whether current partition is selected.
  void setSelected(bool selected);

 private:
  void initUI();

  const Device::Ptr device_;
  QLabel* os_label_ = nullptr;
  bool selected_ = false;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_SIMPLE_DISK_BUTTON_H
