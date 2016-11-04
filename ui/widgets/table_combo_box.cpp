// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/table_combo_box.h"

#include "base/file_util.h"
#include "ui/delegates/table_combo_box_delegate.h"
#include "ui/utils/widget_util.h"

namespace installer {

TableComboBox::TableComboBox(QWidget* parent) : QComboBox(parent) {
  this->setObjectName(QStringLiteral("table_combo_box"));

  this->setFixedSize(240, 36);
  QAbstractItemDelegate* delegate = new TableComboBoxDelegate(this);
  this->setItemDelegate(delegate);
  this->setStyleSheet(ReadFile(":/styles/table_combo_box.css"));

  SetQComboBoxTransparent(this);
}

}  // namespace installer