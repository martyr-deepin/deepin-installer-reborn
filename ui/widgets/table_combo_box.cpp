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

#include "ui/widgets/table_combo_box.h"

#include "base/file_util.h"
#include "ui/delegates/table_combo_box_delegate.h"
#include "ui/utils/widget_util.h"

namespace installer {

TableComboBox::TableComboBox(QWidget* parent) : QComboBox(parent) {
  this->setObjectName("table_combo_box");

  // Same as PartitionSizeSlider.
  this->setFixedSize(310, 36);
  QAbstractItemDelegate* delegate = new TableComboBoxDelegate(this);
  this->setItemDelegate(delegate);
  this->setStyleSheet(ReadFile(":/styles/table_combo_box.css"));

  SetQComboBoxTransparent(this);
}

}  // namespace installer