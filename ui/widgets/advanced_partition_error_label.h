// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
