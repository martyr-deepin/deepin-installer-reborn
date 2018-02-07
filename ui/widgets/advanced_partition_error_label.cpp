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

#include "ui/widgets/advanced_partition_error_label.h"

namespace installer {

AdvancedPartitionErrorLabel::AdvancedPartitionErrorLabel(QWidget* parent)
    : QLabel(parent), state_(AdvancedValidateState::Ok) {
}

void AdvancedPartitionErrorLabel::enterEvent(QEvent* event) {
  QLabel::enterEvent(event);
  emit this->entered();
}

void AdvancedPartitionErrorLabel::leaveEvent(QEvent* event) {
  QLabel::leaveEvent(event);
  emit this->leaved();
}

}  // namespace installer