// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/table_combo_box.h"

#include "base/file_util.h"
#include "ui/delegates/frameless_list_delegate.h"

namespace installer {

TableComboBox::TableComboBox(QWidget* parent) : QComboBox(parent) {
  this->setObjectName(QStringLiteral("table_combo_box"));

  this->setFixedSize(240, 36);
  QAbstractItemDelegate* delegate = new FramelessItemDelegate(this);
  this->setItemDelegate(delegate);
  this->setFrame(false);
  this->setStyleSheet(ReadTextFileContent(":/styles/table_combo_box.css"));
}

}  // namespace installer