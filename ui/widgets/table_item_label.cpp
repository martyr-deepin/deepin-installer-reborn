// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/table_item_label.h"

#include "base/file_util.h"

namespace installer {

TableItemLabel::TableItemLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent) {
  this->setObjectName(QStringLiteral("table_item_label"));

  this->setAlignment(Qt::AlignRight);
  this->setStyleSheet(ReadTextFileContent(":/styles/table_item_label.css"));
}

}  // namespace installer