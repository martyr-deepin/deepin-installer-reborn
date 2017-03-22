// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/advanced_partition_error_label.h"

namespace installer {

AdvancedPartitionErrorLabel::AdvancedPartitionErrorLabel(QWidget* parent)
    : QLabel(parent), state_(AdvancedValidateState::Ok) {
}

void AdvancedPartitionErrorLabel::enterEvent(QEvent* event) {
  QLabel::enterEvent(event);
  emit this->hovered();
}

}  // namespace installer