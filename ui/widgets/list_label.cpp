// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/list_label.h"

#include "base/file_util.h"

namespace installer {

ListLabel::ListLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent) {
  this->setObjectName(QStringLiteral("list_label"));
  this->setWordWrap(true);
  this->setStyleSheet(ReadTextFileContent(":/styles/list_label.css"));
}

}  // namespace installer