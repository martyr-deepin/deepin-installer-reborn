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

#ifndef INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ERROR_LABEL_H
#define INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ERROR_LABEL_H

#include <QLabel>

#include "ui/delegates/advanced_validate_state.h"

namespace installer {

// To display error message in AdvancedPartitionFrame.
class AdvancedPartitionErrorLabel : public QLabel {
  Q_OBJECT

 public:
  explicit AdvancedPartitionErrorLabel(QWidget* parent = nullptr);

  // Get or set validate state.
  void setValidateState(AdvancedValidateState state) { state_ = state; }
  AdvancedValidateState state() const { return state_; }

 signals:
  // Emitted when mouse moved enter widget area.
  void entered();

  // Emitted when mouse moved out of widget area.
  void leaved();

 protected:
  void enterEvent(QEvent* event) override;
  void leaveEvent(QEvent* event) override;

 private:
  AdvancedValidateState state_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ERROR_LABEL_H
