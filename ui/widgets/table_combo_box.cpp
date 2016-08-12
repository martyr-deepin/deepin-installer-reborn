// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/table_combo_box.h"

#include "base/file_util.h"

namespace ui {

TableComboBox::TableComboBox(QWidget* parent) : QComboBox(parent) {
  this->setObjectName(QStringLiteral("table_combo_box"));

  this->setStyleSheet(
      base::ReadTextFileContent(":/styles/table_combo_box.css"));
}

}  // namespace ui